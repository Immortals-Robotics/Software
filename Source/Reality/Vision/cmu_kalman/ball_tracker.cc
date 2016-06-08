/* LICENSE:
  =========================================================================
    CMDragons'02 RoboCup F180 Source Code Release
  -------------------------------------------------------------------------
    Copyright (C) 2002 Manuela Veloso, Brett Browning, Mike Bowling,
                       James Bruce; {mmv, brettb, mhb, jbruce}@cs.cmu.edu
    School of Computer Science, Carnegie Mellon University
  -------------------------------------------------------------------------
    This software is distributed under the GNU General Public License,
    version 2.  If you do not have a copy of this licence, visit
    www.gnu.org, or write: Free Software Foundation, 59 Temple Place,
    Suite 330 Boston, MA 02111-1307 USA.  This program is distributed
    in the hope that it will be useful, but WITHOUT ANY WARRANTY,
    including MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  ------------------------------------------------------------------------- */

#include <stdio.h>
#include <math.h>

#include "constants.h"
#include "ball_tracker.h"
#include "configreader.h"

// Parameters
static bool cr_setup = false;
CR_DECLARE(BALL_WALLS_SLOPED);
CR_DECLARE(BALL_WALLS_OOB);
CR_DECLARE(BALL_IMPROBABILITY_FILTERING);
CR_DECLARE(BALL_TEAMMATE_COLLISION_RADIUS);
CR_DECLARE(BALL_OPPONENT_COLLISION_RADIUS);

CR_DECLARE(BALL_PRINT_KALMAN_ERROR);
CR_DECLARE(BALL_POSITION_VARIANCE);
CR_DECLARE(BALL_VELOCITY_VARIANCE_NO_ROBOT);
CR_DECLARE(BALL_VELOCITY_VARIANCE_NEAR_ROBOT);
CR_DECLARE(BALL_FRICTION);
CR_DECLARE(BALL_CONFIDENCE_THRESHOLD);
CR_DECLARE(BALL_OCCLUDE_TIME);
CR_DECLARE(BALL_LIKELIHOOD_THRESHOLD);

//
// State = ( x, y, v_x, v_y )
// Observation = ( x, y )
//
// Noise_Propagate = ( v_x, v_y )
// Noise_Observe = ( x, y )
//

BallTracker::BallTracker() : Kalman(4, 2, FRAME_PERIOD)
{
  _reset = 1;
  occluded = Visible;
  tracker = NULL;

  if (!cr_setup) {
    CR_SETUP(tracker, BALL_WALLS_SLOPED, CR_INT);
    CR_SETUP(tracker, BALL_WALLS_OOB, CR_INT);
    CR_SETUP(tracker, BALL_IMPROBABILITY_FILTERING, CR_INT);
    CR_SETUP(tracker, BALL_TEAMMATE_COLLISION_RADIUS, CR_DOUBLE);
    CR_SETUP(tracker, BALL_OPPONENT_COLLISION_RADIUS, CR_DOUBLE);
    
    CR_SETUP(tracker, BALL_PRINT_KALMAN_ERROR, CR_INT);
    CR_SETUP(tracker, BALL_POSITION_VARIANCE, CR_DOUBLE);
    CR_SETUP(tracker, BALL_VELOCITY_VARIANCE_NO_ROBOT, CR_DOUBLE);
    CR_SETUP(tracker, BALL_VELOCITY_VARIANCE_NEAR_ROBOT, CR_DOUBLE);
    CR_SETUP(tracker, BALL_FRICTION, CR_DOUBLE);
    CR_SETUP(tracker, BALL_CONFIDENCE_THRESHOLD, CR_DOUBLE);
    CR_SETUP(tracker, BALL_OCCLUDE_TIME, CR_DOUBLE);
    CR_SETUP(tracker, BALL_LIKELIHOOD_THRESHOLD, CR_DOUBLE);

    cr_setup = true;
  }

  if (IVAR(BALL_PRINT_KALMAN_ERROR))
    prediction_lookahead = LATENCY_DELAY;
}

double BallTracker::velocity_variance(const Matrix &x)
{
  if (!tracker) return DVAR(BALL_VELOCITY_VARIANCE_NEAR_ROBOT);

  vector2d ball = vector2d(x.e(0, 0), x.e(1, 0));
  double dist = 5000.0;

  for(int i = 0; i < NUM_TEAMS; i++) {
    for(int j = 0; j < MAX_TEAM_ROBOTS; j++) {
      if (!tracker->Exists(i, j)) continue;

      double d = (tracker->robots[i][j].position(0.0) - ball).length();
      if (d < dist) dist = d;
    }
  }

  double r = bound((dist - ROBOT_DEF_WIDTH_H) / ROBOT_DEF_WIDTH_H, 0, 1);
  return r * DVAR(BALL_VELOCITY_VARIANCE_NO_ROBOT) +
    (1 - r) * DVAR(BALL_VELOCITY_VARIANCE_NEAR_ROBOT);
}

bool BallTracker::check_occlusion()
{
  if (!tracker) return false;

  if (occluded != Visible) return true;

  vector2d camera(0,0);
  vector2d ball = position(0.0) - camera;

  double occluding_pct = 0.5;

  for(int i = 0; i < NUM_TEAMS; i++) {
    for(int j = 0; j < MAX_TEAM_ROBOTS; j++) {
      if (!tracker->Exists(i, j)) continue;

      double radius = tracker->Radius(i, j);
      double height = tracker->Height(i, j);
      
      vector2d p = tracker->robots[i][j].position(0.0) - camera;
      double from = offset_to_line(vector2d(0, 0), ball, p);
      double along = offset_along_line(vector2d(0, 0), ball, p);
      double ball_along = ball.length();
      
      if (fabs(from) > radius) continue;
      if (ball_along < along) continue;
      
      along += sqrt(radius * radius - from * from);
      
      double x = (along * height) / (CAMERA_HEIGHT - height);
      double pct = (x - (ball_along - along) + BALL_RADIUS) / 
	(2.0 * BALL_RADIUS);

      pct = bound(pct, 0, 1);

      if (pct > occluding_pct) {
	occluded = MaybeOccluded;
	occluding_team = i;
	occluding_robot = j;
	occluding_offset = (ball - p).rotate(-(p - camera).angle());
	
	occluding_pct = pct;
      }
    }

  }
  
  return (occluded != Visible);
}

void BallTracker::tick_occlusion(double dt)
{
  vector2d camera(0, 0);

  vector2d p = tracker->robots[occluding_team][occluding_robot]
    .position(0.0);
  vector2d v = tracker->robots[occluding_team][occluding_robot]
    .velocity(0.0);
  vector2d b = occluding_offset.rotate((p - camera).angle());
  
  double bdelta = MAX(v.dot(b.norm()) * dt, 0.0);
  double radius;

  radius = DVAR(BALL_TEAMMATE_COLLISION_RADIUS);

  if (b.length() - bdelta < radius) b = b.norm(radius);
  else b = b.norm(b.length() - bdelta);

  occluding_offset = b.rotate(-(p - camera).angle());

  // Update the x and P queue.
  Matrix x(4,1), P(4);
  vector2d xp = occluded_position(dt);
  vector2d xv = occluded_velocity(dt);

  x.e(0,0) = xp.x;
  x.e(1,0) = xp.y;
  x.e(2,0) = xv.x;
  x.e(3,0) = xv.y;

  P.e(0,0) *= DVAR(BALL_POSITION_VARIANCE);
  P.e(1,1) *= DVAR(BALL_POSITION_VARIANCE);
  P.e(2,2) *= 250000.0; // 500m/s
  P.e(3,3) *= 250000.0; // 500m/s

  xs.clear(); xs.push_back(x);
  Ps.clear(); Ps.push_back(P);
  time += dt;
}

vector2d BallTracker::occluded_position(double time)
{
  if (!tracker) return vector2d(0.0, 0.0);

  vector2d camera(0, 0);
  vector2d b;

  b = tracker->robots[occluding_team][occluding_robot].position(time);
  b += occluding_offset.rotate((b - camera).angle());

  return b;
}

vector2d BallTracker::occluded_velocity(double time)
{
  if (!tracker) return vector2d(0.0, 0.0);
  return tracker->robots[occluding_team][occluding_robot].velocity(time);
}

void BallTracker::observe(vraw obs, double timestamp)
{
  // mhb: Need this?
  if (isnan(xs[0].e(0,0))) _reset = true;

  if (_reset && obs.timestamp >= timestamp &&
      obs.conf >= DVAR(BALL_CONFIDENCE_THRESHOLD)) {
    Matrix x(4,1), P(4);

    x.e(0,0) = obs.pos.x;
    x.e(1,0) = obs.pos.y;
    x.e(2,0) = 0.0;
    x.e(3,0) = 0.0;

    P.e(0,0) *= DVAR(BALL_POSITION_VARIANCE);
    P.e(1,1) *= DVAR(BALL_POSITION_VARIANCE);
    P.e(2,2) *= 250000.0; // 500m/s
    P.e(3,3) *= 250000.0; // 500m/s

    initial(obs.timestamp, x, P);

    occluded = Visible;

    _reset = false;

  } else {

    if (_reset && occluded != Occluded) return;

    // If this is a new observation.
    if (timestamp > time) {

      // Tick to current time.
      if (occluded == Occluded) {
	tick_occlusion(timestamp - time);
      } else {
	tick(timestamp - time);
      }

      // Make Observation Matrix
      Matrix o(2,1);
      o.e(0,0) = obs.pos.x;
      o.e(1,0) = obs.pos.y;
      
      if (IVAR(BALL_IMPROBABILITY_FILTERING)) {
	// Check for improbable observations (i.e. noise)
	if (obs_likelihood(0.0, o) <= DVAR(BALL_LIKELIHOOD_THRESHOLD)) 
	  obs.conf = -1.0;
      }

      // Make observation
      if (obs.timestamp == timestamp && 
	  obs.conf > DVAR(BALL_CONFIDENCE_THRESHOLD)) {
	update(o);
	
	occluded = Visible;
	occluded_last_obs_time = obs.timestamp;

      } else {

	if (occluded == Visible)
	  check_occlusion();

	if (occluded == MaybeOccluded && 
	    timestamp - occluded_last_obs_time > DVAR(BALL_OCCLUDE_TIME)) {

	  occluded = Occluded;
	  _reset = true;
	}
      }

      if (error_time_elapsed() > 10.0) {
	fprintf(stderr, "Kalman Error (pos, vpos): ");
	fprintf(stderr, "%f ", 
		hypot(error_mean().e(0, 0), error_mean().e(1, 0)));
	fprintf(stderr, "%f\n", 
		hypot(error_mean().e(2, 0), error_mean().e(3, 0)));
	error_reset();
      }

    }
  }

}
void BallTracker::reset(double timestamp, float state[4], float variances[16],
			OccludeFlag _occluded, 
			char _occluding_team, char _occluding_robot,
			vector2f _occluding_offset)
{
  Matrix x(4,1,state), P(4,4,variances);

  initial(timestamp, x, P);
  
  occluded = _occluded;
  occluding_team = _occluding_team;
  occluding_robot = _occluding_robot;
  occluding_offset = vector2d(_occluding_offset.x, _occluding_offset.y);
  
  _reset = false;
}

vector2d BallTracker::position(double time)
{  
  if (occluded == Occluded) return occluded_position(time);

  Matrix x = predict(time);
  return vector2d(x.e(0,0), x.e(1,0));
}

vector2d BallTracker::velocity(double time)
{  
  if (occluded == Occluded) return occluded_velocity(time);

  Matrix x = predict(time);
  return vector2d(x.e(2,0), x.e(3,0));
}

Matrix BallTracker::covariances(double time)
{  
  return predict_cov(time);
}

bool BallTracker::collision(double time, int &team, int &robot) 
{
  Matrix I = predict_info(time);

  if (I.nrows() <= 1) return false;

  team = (int) rint(I.e(1, 0));
  robot = (int) rint(I.e(1, 0));

  return true;
}

#ifndef MIN
#define MIN(a,b) ((a<b) ? a : b)
#endif

Matrix& BallTracker::f(const Matrix &x, Matrix &I)
{
  I = Matrix();

  static Matrix f;

  f = x; // Copy Matrix
  double &_x = f.e(0,0), &_y = f.e(1,0), &_vx = f.e(2,0), &_vy = f.e(3,0);
  double _v = sqrt(_vx * _vx + _vy * _vy);

  double _a = MIN(DVAR(BALL_FRICTION) * GRAVITY, _v / stepsize);
  double _ax = (_v == 0.0) ? 0.0 : -_a * _vx / _v;
  double _ay = (_v == 0.0) ? 0.0 : -_a * _vy / _v;

  double walls = false;

  if (IVAR(BALL_WALLS_SLOPED)) {
    if (fabs(_x) > FIELD_LENGTH_H && fabs(_y) > GOAL_WIDTH_H) {
      _ax += copysign(M_SQRT1_2 * GRAVITY * 5.0 / 7.0, -_x);
      walls = true;
    }
    
    if (fabs(_y) > FIELD_WIDTH_H) {
      _ay = copysign(M_SQRT1_2 * GRAVITY * 5.0 / 7.0, -_y);
      walls = true;
    }
  }

  if (IVAR(BALL_WALLS_OOB)) {
    if ((fabs(_x) > FIELD_LENGTH_H + WALL_WIDTH && 
	 fabs(_y) > GOAL_WIDTH_H) ||
	(fabs(_y) > FIELD_WIDTH_H + WALL_WIDTH)) {
      _vx = 0.0;
      _vy = 0.0;
      _ax = 0.0;
      _ay = 0.0;

      walls = true;
    }
  }

  // Update Position
  _x += _vx * stepsize + 0.5 * _ax * stepsize * stepsize;
  _y += _vy * stepsize + 0.5 * _ay * stepsize * stepsize;

  // If there's a collision... then set ball's velocity to the colliding
  //  object's velocity.
  vector2d cv, cp;
  int team, robot;

  if (!walls && check_for_collision(f, cp, cv, team, robot)) {
    _vx = cv.x; _vy = cv.y;
    I = Matrix(2, 1);
    I.e(0, 0) = team;
    I.e(1, 0) = robot;
  } else {
    _vx += _ax * stepsize;
    _vy += _ay * stepsize;
  }

  return f;
}

bool BallTracker::check_for_collision(const Matrix &x, 
				      vector2d &cp, vector2d &cv,
				      int &team, int &robot)
{
  if (!tracker) return false;

  vector2d bp = vector2d(x.e(0,0), x.e(1,0));
  vector2d bv = vector2d(x.e(2,0), x.e(3,0));

  double dist = 5000.0;
  bool rv = false;

  for(int i = 0; i < NUM_TEAMS; i++) {
    for(int j = 0; j < MAX_TEAM_ROBOTS; j++) {
      if (!tracker->Exists(i, j)) continue;

      double radius;

      radius = DVAR(BALL_TEAMMATE_COLLISION_RADIUS);

      if (radius <= 0) continue;

      vector2d p = tracker->robots[i][j].position(stepped_time - time);
      vector2d v = tracker->robots[i][j].velocity(stepped_time - time);
      double d = MIN((p - bp).length(),
		     (p + v * stepsize - bp + bv * stepsize).length());
					
      // Ball is within radius, nothing else is closest, and ball is
      //  moving towards the robot...  Count as collision.
      if (d <= radius && d < dist && (bv - v).dot(p - bp) > 0.0) {
	cp = p + (bp - p).norm(radius);
	cv = v; rv = true; dist = d;
	team = i; robot = j;
      }
    }
  }

  return rv;
}

Matrix& BallTracker::h(const Matrix &x)
{
  static Matrix h(2,1);
  h.e(0,0) = x.e(0,0);
  h.e(1,0) = x.e(1,0);
  return h;
}

Matrix& BallTracker::Q(const Matrix &x)
{
  static Matrix Q(2);

  // Base noise covariances on distance to nearest robot.
  Q.e(0, 0) = Q.e(1, 1) = velocity_variance(x);

  return Q;
}

Matrix& BallTracker::R(const Matrix &x)
{
  static Matrix R;

  if (!R.nrows()) {
    R.identity(2); R.scale(DVAR(BALL_POSITION_VARIANCE));
  }

  return R;
}

Matrix& BallTracker::A(const Matrix &x)
{
  static Matrix A;

  // This is not quite right since this doesn't account for friction
  // But the Jacobian with friction is pretty messy.

  if (!A.nrows()) {
    A.identity(4); 
    A.e(0,2) = stepsize;
    A.e(1,3) = stepsize;
  }

  return A;
}

Matrix& BallTracker::W(const Matrix &x)
{
  static Matrix W("[0, 0; 0, 0; 1, 0; 0, 1]");
  return W;
}

Matrix& BallTracker::H(const Matrix &x)
{
  static Matrix H("[ 1, 0, 0, 0; "
		  "  0, 1, 0, 0 ] ");
  return H;
}

Matrix& BallTracker::V(const Matrix &x)
{
  static Matrix V("[ 1, 0; "
		  "  0, 1 ]");

  return V;
}


