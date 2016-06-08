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
#include <cstdlib>

#include "constants.h"

#include "robot_tracker.h"
#include "configreader.h"


static bool cr_setup = false;
CR_DECLARE(ROBOT_PRINT_KALMAN_ERROR);
CR_DECLARE(ROBOT_FAST_PREDICT);
CR_DECLARE(ROBOT_USE_AVERAGES_IN_PROPAGATION);
CR_DECLARE(ROBOT_CONFIDENCE_THRESHOLD);
CR_DECLARE(ROBOT_POSITION_VARIANCE);
CR_DECLARE(ROBOT_THETA_VARIANCE);
CR_DECLARE(ROBOT_NONE_VELOCITY_VARIANCE);
CR_DECLARE(ROBOT_NONE_ANGVEL_VARIANCE);
CR_DECLARE(ROBOT_DIFF_VELOCITY_VARIANCE);
CR_DECLARE(ROBOT_DIFF_VELOCITY_VARIANCE_PERP);
CR_DECLARE(ROBOT_DIFF_ANGVEL_VARIANCE);
CR_DECLARE(ROBOT_OMNI_VELOCITY_VARIANCE);
CR_DECLARE(ROBOT_OMNI_ANGVEL_VARIANCE);
CR_DECLARE(ROBOT_STUCK_VARIANCE);
CR_DECLARE(ROBOT_VELOCITY_NEXT_STEP_COVARIANCE);
CR_DECLARE(ROBOT_STUCK_DECAY);
CR_DECLARE(ROBOT_STUCK_THRESHOLD);

//
// State = ( x, y, theta, v_par, v_perp, v_theta, stuck )
// Observation = ( x, y, theta )
//
// Noise_Propagate = ( v_par, v_perp, v_theta, stuck )
// Noise_Update = ( x, y, theta )
//

RobotTracker::RobotTracker(int _type, double _latency) 
  : Kalman(7, 3, FRAME_PERIOD)
{
  if (!cr_setup) {
    CR_SETUP(tracker, ROBOT_PRINT_KALMAN_ERROR, CR_DOUBLE);
    CR_SETUP(tracker, ROBOT_FAST_PREDICT, CR_INT);
    CR_SETUP(tracker, ROBOT_USE_AVERAGES_IN_PROPAGATION, CR_INT);
    CR_SETUP(tracker, ROBOT_CONFIDENCE_THRESHOLD, CR_DOUBLE);
    CR_SETUP(tracker, ROBOT_POSITION_VARIANCE, CR_DOUBLE);
    CR_SETUP(tracker, ROBOT_THETA_VARIANCE, CR_DOUBLE);
    CR_SETUP(tracker, ROBOT_NONE_VELOCITY_VARIANCE, CR_DOUBLE);
    CR_SETUP(tracker, ROBOT_NONE_ANGVEL_VARIANCE, CR_DOUBLE);
    CR_SETUP(tracker, ROBOT_DIFF_VELOCITY_VARIANCE, CR_DOUBLE);
    CR_SETUP(tracker, ROBOT_DIFF_VELOCITY_VARIANCE_PERP, CR_DOUBLE);
    CR_SETUP(tracker, ROBOT_DIFF_ANGVEL_VARIANCE, CR_DOUBLE);
    CR_SETUP(tracker, ROBOT_OMNI_VELOCITY_VARIANCE, CR_DOUBLE);
    CR_SETUP(tracker, ROBOT_OMNI_ANGVEL_VARIANCE, CR_DOUBLE);
    CR_SETUP(tracker, ROBOT_STUCK_VARIANCE, CR_DOUBLE);
    CR_SETUP(tracker, ROBOT_VELOCITY_NEXT_STEP_COVARIANCE, CR_DOUBLE);
    CR_SETUP(tracker, ROBOT_STUCK_DECAY, CR_DOUBLE);
    CR_SETUP(tracker, ROBOT_STUCK_THRESHOLD, CR_DOUBLE);
    cr_setup = true;
  }

  type = _type;
  latency = _latency;
  reset_on_obs = 1;

  if (IVAR(ROBOT_PRINT_KALMAN_ERROR)) 
    prediction_lookahead = LATENCY_DELAY;
}

RobotTracker::rcommand RobotTracker::get_command(double time)
{
  if (cs.empty() || cs[0].timestamp > time)
    return (rcommand) { 0.0, vector3d(0.0, 0.0, 0.0) };

  unsigned int i;

  for(i = 1; i < cs.size(); i++) {
    if (cs[i].timestamp > time) break;
  }
  
  return cs[i-1];

}

void RobotTracker::command(double timestamp, vector3d vs)
{
  rcommand c = { timestamp + latency - (FRAME_PERIOD / 2.0), vs };

  while(cs.size() > 1 && cs[0].timestamp < time - stepsize)
    cs.pop_front();

  while(!cs.empty() && cs.back().timestamp == c.timestamp)
    cs.pop_back();

  cs.push_back(c);
}

void RobotTracker::observe(vraw obs, double timestamp)
{
  if (reset_on_obs) {
    if (obs.conf <= 0.0) return;

    static Matrix x(7,1), P(7);

    x.e(0,0) = obs.pos.x;
    x.e(1,0) = obs.pos.y;
    x.e(2,0) = obs.angle;
    x.e(3,0) = 0.0;
    x.e(4,0) = 0.0;
    x.e(5,0) = 0.0;
    x.e(6,0) = 0.0;

    P.e(0,0) = DVAR(ROBOT_POSITION_VARIANCE);
    P.e(1,1) = DVAR(ROBOT_POSITION_VARIANCE);
    P.e(2,2) = DVAR(ROBOT_THETA_VARIANCE);
    P.e(3,3) = 0.0; // 0m/s
    if (type == ROBOT_TYPE_DIFF) P.e(4,4) = 0.0; 
    else P.e(4,4) = 0.0; // 0m/s
    P.e(5,5) = 0.0;
    P.e(6,6) = 0.0;

    initial(obs.timestamp, x, P);

    reset_on_obs = false;

  } else {

    // If this is a new observation.
    if (timestamp > time) {

      // Tick to current time.
      tick(timestamp - time);

      // Make observation
      if (obs.timestamp == timestamp) {
	double xtheta = xs.front().e(2,0);
	
	Matrix o(3,1);
	o.e(0,0) = obs.pos.x;
	o.e(1,0) = obs.pos.y;
	o.e(2,0) = anglemod(obs.angle - xtheta) + xtheta;

	update(o);
      }

      if (error_time_elapsed() > 10.0) {
	fprintf(stderr, "Kalman Error (pos, theta, vpos, vtheta): ");
	fprintf(stderr, "%f ", 
		hypot(error_mean().e(0, 0), error_mean().e(1, 0)));
	fprintf(stderr, "%f ", error_mean().e(2, 0));
	fprintf(stderr, "%f ", 
		hypot(error_mean().e(3, 0), error_mean().e(4, 0)));
	fprintf(stderr, "%f\n", error_mean().e(5, 0));
	error_reset();
      }
    }
  }
}

void RobotTracker::reset(double timestamp, float state[7])
{
  Matrix x(7,1,state), P(7);

  P.e(0,0) = DVAR(ROBOT_POSITION_VARIANCE);
  P.e(1,1) = DVAR(ROBOT_POSITION_VARIANCE);
  P.e(2,2) = DVAR(ROBOT_THETA_VARIANCE);
  P.e(3,3) = 0.0; // 0m/s
  if (type == ROBOT_TYPE_DIFF) P.e(4,4) = 0.0; 
  else P.e(4,4) = 0.0; // 0m/s
  P.e(5,5) = 0.0;
  P.e(6,6) = 0.0;

  initial(timestamp, x, P);
  
  reset_on_obs = false;
}

vector2d RobotTracker::position(double time)
{
  if (IVAR(ROBOT_FAST_PREDICT)) {
    if (time > latency) {
      Matrix x = predict(latency);
      return vector2d(x.e(0,0), x.e(1,0)) + 
	vector2d(x.e(3,0), x.e(4,0)) * (time - latency);
    } else {
      Matrix x = predict(time);
      return vector2d(x.e(0,0), x.e(1,0));
    }
  } else {
    Matrix x = predict(time);
    return vector2d(x.e(0,0), x.e(1,0));
  }
}

vector2d RobotTracker::velocity(double time)
{
  Matrix x;

  if (IVAR(ROBOT_FAST_PREDICT)) {
    if (time > latency) {
      x = predict(latency);
    } else {
      x = predict(time);
    }
  } else 
    x = predict(time);


  double a = x.e(2,0);
  double c = cos(a);
  double s = sin(a);

  double vx = x.e(3,0);
  double vy = x.e(4,0);

  double stuck = x.e(6,0);

  if (stuck > DVAR(ROBOT_STUCK_THRESHOLD)) 
    return vector2d(0.0, 0.0);

  return vector2d(c * vx - s * vy, s * vx + c * vy);
}

// return the velocity un-rotate
vector2d RobotTracker::velocity_raw(double time)
{
  Matrix x;
    
  if (IVAR(ROBOT_FAST_PREDICT)) {
    if (time > latency) {
      x = predict(latency);
    } else {
      x = predict(time);
    }
  } else
    x = predict(time);

  double vx = x.e(3,0);
  double vy = x.e(4,0);

  double stuck = x.e(6,0);

  if (stuck > 0.8) return vector2d(0.0, 0.0);

  return vector2d(vx, vy);
}


double RobotTracker::direction(double time)
{
  Matrix x = predict(time);
  return x.e(2,0);
}

double RobotTracker::angular_velocity(double time)
{
  Matrix x = predict(time);
  return x.e(5,0);
}

double RobotTracker::stuck(double time)
{
  Matrix x = predict(time);
  return bound(x.e(6,0), 0, 1);
}

Matrix& RobotTracker::f(const Matrix &x, Matrix &I)
{
  I = Matrix();

  static Matrix f;
  f = x;

  rcommand c = get_command(stepped_time);

  double 
    &_x = f.e(0,0), 
    &_y = f.e(1,0), 
    &_theta = f.e(2,0), 
    &_vpar = f.e(3,0), 
    &_vperp = f.e(4,0),
    &_vtheta = f.e(5,0),
    &_stuck = f.e(6,0);

  _stuck = bound(_stuck, 0, 1) * DVAR(ROBOT_STUCK_DECAY);

  double avg_vpar = 0, avg_vperp = 0, avg_vtheta = 0, avg_theta = 0;
  double avg_weight = 0.5;

  if (IVAR(ROBOT_USE_AVERAGES_IN_PROPAGATION)) {
    avg_vpar = avg_weight * _vpar;
    avg_vperp = avg_weight * _vperp;
    avg_vtheta = avg_weight * _vtheta;
  }

  if (type != ROBOT_TYPE_NONE) {
    _vpar = c.vs.x;
    _vperp = c.vs.y;
    _vtheta = c.vs.z;
  }
  
  if (IVAR(ROBOT_USE_AVERAGES_IN_PROPAGATION)) {
    avg_vpar += (1.0 - avg_weight) * _vpar;
    avg_vperp += (1.0 - avg_weight) * _vperp;
    avg_vtheta += (1.0 - avg_weight) * _vtheta;
    
    avg_theta = avg_weight * _theta;
    
    _theta += (1.0 - _stuck) * stepsize * avg_vtheta;
    
    avg_theta += (1.0 - avg_weight) * _theta;
    
    _x += (1.0 - _stuck) * stepsize * 
      (avg_vpar * cos(avg_theta) + avg_vperp * -sin(avg_theta));
    _y += (1.0 - _stuck) * stepsize * 
      (avg_vpar * sin(avg_theta) + avg_vperp * cos(avg_theta));
  } else {
    _theta += (1.0 - _stuck) * stepsize * _vtheta;
    _x += (1.0 - _stuck) * stepsize * 
      (_vpar * cos(_theta) + _vperp * -sin(_theta));
    _y += (1.0 - _stuck) * stepsize * 
      (_vpar * sin(_theta) + _vperp * cos(_theta));
  }

  _theta = anglemod(_theta);

  return f;
}

Matrix& RobotTracker::h(const Matrix &x)
{
  static Matrix h(3,1);

  h.e(0,0) = x.e(0,0);
  h.e(1,0) = x.e(1,0);
  h.e(2,0) = x.e(2,0);

  return h;
}

Matrix& RobotTracker::Q(const Matrix &x)
{
  static Matrix Q(4);

  switch (type) {
  case ROBOT_TYPE_DIFF:
    Q.e(0,0) = DVAR(ROBOT_DIFF_VELOCITY_VARIANCE);
    Q.e(1,1) = DVAR(ROBOT_DIFF_VELOCITY_VARIANCE_PERP);
    Q.e(2,2) = DVAR(ROBOT_DIFF_ANGVEL_VARIANCE);
    Q.e(3,3) = DVAR(ROBOT_STUCK_VARIANCE);
    break;

  case ROBOT_TYPE_OMNI:
    Q.e(0,0) = DVAR(ROBOT_OMNI_VELOCITY_VARIANCE);
    Q.e(1,1) = DVAR(ROBOT_OMNI_VELOCITY_VARIANCE);
    Q.e(2,2) = DVAR(ROBOT_OMNI_ANGVEL_VARIANCE);
    Q.e(3,3) = DVAR(ROBOT_STUCK_VARIANCE);
    break;

  case ROBOT_TYPE_NONE:
    Q.e(0,0) = DVAR(ROBOT_NONE_VELOCITY_VARIANCE);
    Q.e(1,1) = DVAR(ROBOT_NONE_VELOCITY_VARIANCE);
    Q.e(2,2) = DVAR(ROBOT_NONE_ANGVEL_VARIANCE);
    Q.e(3,3) = 0.0;
    break;
  }

  return Q;
}

Matrix& RobotTracker::R(const Matrix &x)
{
  static Matrix R;

  if (!R.nrows()) {
    R.identity(3); 
    R.e(0,0) = (DVAR(ROBOT_POSITION_VARIANCE));
    R.e(1,1) = (DVAR(ROBOT_POSITION_VARIANCE));
    R.e(2,2) = (DVAR(ROBOT_THETA_VARIANCE));
  }

  return R;
}

Matrix& RobotTracker::A(const Matrix &x)
{
  static Matrix A(7);

  double theta = x.e(2,0);
  double vpar = x.e(3,0), vperp = x.e(4,0), vtheta = x.e(5,0);
  double stuck = x.e(6,0);
  double cos_theta = cos(theta), sin_theta = sin(theta);

  A.e(0,2) = (1.0 - stuck) * stepsize * 
    (vpar * -sin_theta + vperp * -cos_theta);
  A.e(0,3) = cos_theta * (1.0 - stuck) * stepsize;
  A.e(0,4) = -sin_theta * (1.0 - stuck) * stepsize;
  A.e(0,6) = -stepsize * (vpar * cos_theta + vperp * -sin_theta);
  A.e(1,2) = (1.0 - stuck) * stepsize * 
    (vpar * cos_theta + vperp * -sin_theta);
  A.e(1,3) = sin_theta * (1.0 - stuck) * stepsize;
  A.e(1,4) = cos_theta * (1.0 - stuck) * stepsize;
  A.e(1,6) = -stepsize * (vpar * sin_theta + vperp * cos_theta);
  A.e(2,5) = (1.0 - stuck) * stepsize;
  A.e(2,6) = -stepsize * vtheta;
  A.e(3,3) = DVAR(ROBOT_VELOCITY_NEXT_STEP_COVARIANCE);
  A.e(4,4) = DVAR(ROBOT_VELOCITY_NEXT_STEP_COVARIANCE);
  A.e(5,5) = DVAR(ROBOT_VELOCITY_NEXT_STEP_COVARIANCE);
  A.e(6,6) = DVAR(ROBOT_STUCK_DECAY);

  return A;
}

Matrix& RobotTracker::W(const Matrix &x)
{
  static Matrix W("[ 0, 0, 0, 0; "
                  "  0, 0, 0, 0; "
                  "  0, 0, 0, 0; "
                  "  1, 0, 0, 0; "
                  "  0, 1, 0, 0; "
                  "  0, 0, 1, 0; "
                  "  0, 0, 0, 1 ]");
  return W;
}

Matrix& RobotTracker::H(const Matrix &x)
{
  static Matrix H("[ 1, 0,  0,  0, 0, 0, 0; "
                  "  0, 1,  0,  0, 0, 0, 0; "
                  "  0, 0,  1,  0, 0, 0, 0 ]");
  return H;
}

Matrix& RobotTracker::V(const Matrix &x)
{
  static Matrix V("[ 1, 0, 0; "
		  "  0, 1, 0; "
                  "  0, 0, 1 ]");
  return V;
}
