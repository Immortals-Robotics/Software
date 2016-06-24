// tactic.cc
// 
// Parent class for tactics.
//
// Created by:  Michael Bowling (mhb@cs.cmu.edu)
//
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
#include "helpers.h"
#include "cmu_goto_point.h"

float motion_time_1d(float dx,float vel0,float vel1,
                            float max_vel,float max_accel,
                            float &t_accel,float &t_cruise,float &t_decel)
// Calculates the timings for a trapezoidal velocity profile with
// constant acceleration.  'dx' is the distance to be traveled, and
// 'vel0' and 'vel1' are the desired initial and final velocities.
// 'max_vel' == maximum veloicty, 'max_accel' == maximum acceleration
// the individual timings are returned in the last three parameters:
// 't_accel' is the time spent in the acceleration phase, 't_cruise'
// is the time spent at maximum velocity, and 't_decel' is the time
// spent in the deceleration phase.  Any of these can be 0.  The total
// of the three times is returned by the function.
{
  float tmp,vmax;
  const bool debug = false;

  if(debug){
    printf("  d%f v%f-%f m%f a%f = ",
           dx,vel0,vel1,max_vel,max_accel);
  }

  if(dx < 0){
    dx = -dx;
    vel0 = -vel0;
    vel1 = -vel1;
  }

  if(vel0 > max_vel) vel0 = max_vel;
  if(vel1 > max_vel) vel1 = max_vel;

  // stop
  if(vel0 > vel1){
    t_decel = (vel0 + vel1) / 2*dx;
    if(debug) printf("<%f> ",fabs(vel1 - vel0)/t_decel);
    if(fabs(vel1 - vel0)/t_decel > max_accel){
      t_accel = t_cruise = 0;
      if(debug) printf("\n");
      return(t_decel);
    }
  }

  // calculate time spent at max velocity
  tmp = 2*max_accel*dx + sq(vel0) + sq(vel1);
  t_cruise = (tmp - 2*sq(max_vel)) / (2*max_vel*max_accel);

  if(t_cruise > 0){
    vmax = max_vel;
  }else{
    vmax = sqrt((max_accel*dx + sq(vel0) + sq(vel1))/2);
    t_cruise = 0;
  }

  if(debug) printf("[am%f] ",vmax);

  t_accel = max(vmax - vel0,(float)0.0) / max_accel;
  t_decel = fabs(vmax - vel1) / max_accel;

  if(debug){
    printf("t(%f,%f,%f):%f\n",
           t_accel,t_cruise,t_decel,t_accel+t_cruise+t_decel);
  }

  return(t_accel + t_cruise + t_decel);
}

double max_speed(float dx,float max_a)
// maximum speed we will be willing to go if we are dx away from
// target and have an acceleration of max_a
{
  return(sqrt(2*max_a*dx));
}

#if 0
void Robot::compute_motion_1d(double x0, double v0, double v1,
                              double a_max, double v_max, double a_factor,
                              double &traj_accel, double &traj_time)
{
  float ta,tc,td;

  ta = tc = td = 0.0;
  traj_time = motion_time_1d(x0,v0,v1,v_max,a_max/a_factor,ta,tc,td);

  if(ta > 0.0){
    traj_accel = -a_max * sign(x0);
  }else if(tc > 0.0){
    traj_accel = 0.0;
  }else{
    traj_accel =  a_max * sign(x0);
  }
  printf("  accel=%f\n",traj_accel);
}
#endif

#if 1
void compute_motion_1d(float x0, float v0, float v1,
                       float a_max, float v_max, float a_factor,
                       float &traj_accel, float &traj_time)
{
  // First check to see if nothing needs to be done...
  if (x0 == 0 && v0 == v1) { traj_accel = 0; traj_time = 0;  return; }

  if(!finite(x0) || !finite(v0) || !finite(v1)){
    printf("Robot::compute_motion_1d: NANs!\n");
    traj_accel = 0; traj_time = 0;  return;
  }

  // Need to do some motion.
  a_max /= a_factor;

  float time_to_v1 = fabs(v0 - v1) / a_max;
  float x_to_v1 = fabs((v0 + v1) / 2.0) * time_to_v1;

  float period = 2.0 / 61.0;

  v1 = copysign(v1, -x0);

  if (v0 * x0 > 0 || (fabs(v0) > fabs(v1) && x_to_v1 > fabs(x0))) {
    // Time to reach goal after stopping + Time to stop.
    float time_to_stop = fabs(v0) / a_max;
    float x_to_stop = v0 * v0 / (2 * a_max);

    compute_motion_1d(x0 + copysign(x_to_stop, v0), 0, v1, a_max * a_factor,
                      v_max, a_factor, traj_accel, traj_time);
    traj_time += time_to_stop;
 
    // Decelerate
    if (traj_time < period) 
      traj_accel = compute_stop(v0, a_max * a_factor);
    else if (time_to_stop < period) 
      traj_accel = time_to_stop / period * - copysign(a_max * a_factor, v0) +
	(1.0 - time_to_stop / period) * traj_accel;
    else traj_accel = -copysign(a_max * a_factor, v0);

    return;
  }


  // At this point we have two options.  We can maximally accelerate
  // and then maximally decelerate to hit the target.  Or we could
  // find a single acceleration that would reach the target with zero
  // velocity.  The later is useful when we are close to the target
  // where the former is less stable.

  // OPTION 1
  // This computes the amount of time to accelerate before decelerating.
  float t_a, t_accel, t_decel;

  if (fabs(v0) > fabs(v1)) {
    //    t_a = (sqrt((3*v1*v1 + v0*v0) / 2.0 - fabs(v0 * v1) + fabs(x0) * a_max) 
    //	   - fabs(v0)) / a_max;
    t_a = (sqrt((v0 * v0 + v1 * v1) / 2.0 + fabs(x0) * a_max) 
	   - fabs(v0)) / a_max;

    if (t_a < 0.0) t_a = 0;
    t_accel = t_a;
    t_decel = t_a + time_to_v1;
  } else if (x_to_v1 > fabs(x0)) {
    t_a = (sqrt(v0 * v0 + 2 * a_max * fabs(x0)) - fabs(v0)) / a_max;
    t_accel = t_a;
    t_decel = 0.0;
  } else {
    //    t_a = (sqrt((3*v0*v0 + v1*v1) / 2.0 - fabs(v0 * v1) + fabs(x0) * a_max) 
    //  - fabs(v1)) / a_max;

    t_a = (sqrt((v0 * v0 + v1 * v1) / 2.0 + fabs(x0) * a_max) 
	   - fabs(v1)) / a_max;

    if (t_a < 0.0) t_a = 0;
    t_accel = t_a + time_to_v1;
    t_decel = t_a;
  }

  // OPTION 2
  float a_to_v1_at_x0 = (v0 * v0 - v1 * v1) / (2 * fabs(x0));
  float t_to_v1_at_x0 =
    (-fabs(v0) + sqrt(v0 * v0 + 2 * fabs(a_to_v1_at_x0) * fabs(x0))) / 
    fabs(a_to_v1_at_x0);

  // We follow OPTION 2 if t_a is less than a FRAME_PERIOD making it
  // difficult to transition to decelerating and stopping exactly.
  if (0 && a_to_v1_at_x0 < a_max && a_to_v1_at_x0 > 0.0 &&
      t_to_v1_at_x0 < 2.0 / 61.0 && 0) {

    // OPTION 2
    // Use option 1 time, even though we're not following it.
    traj_time = t_accel + t_decel;;

    // Target acceleration to stop at x0.
    traj_accel = -copysign(a_to_v1_at_x0, v0);

    return;
  } else {

    // OPTION 1
    // Time to accelerate and decelerate.
    traj_time = t_accel + t_decel;

    // If the acceleration time would get the speed above v_max, then
    //  we need to add time to account for cruising at max speed.
    if (t_accel * a_max + fabs(v0) > v_max) {
      traj_time += 
	pow(v_max - (a_max * t_accel + fabs(v0)), 2.0) / a_max / v_max;
    }

    // Accelerate (unless t_accel is less than FRAME_PERIOD, then set
    // acceleration to average acceleration over the period.)
    if (t_accel < period && t_decel == 0.0) {
      traj_accel = copysign(a_max * a_factor, -x0);
    } else if (t_accel < period && t_decel > 0.0) {
      traj_accel = compute_stop(v0, a_max * a_factor);
    } else if (t_accel < period) {
      traj_accel = copysign((2.0 * t_accel / (period) - 1) * a_max * a_factor, v0);
    } else {
      traj_accel = copysign(a_max * a_factor, -x0);
    }
  }
}
#endif

void compute_motion_2d(TVec2 x0, TVec2 v0, TVec2 v1,
                       float a_max, float v_max, float a_factor,
                              TVec2 &traj_accel, float &time)
{
  float time_x, time_y;
  float rotangle;

  if (Magnitude(v1) > 0.0) rotangle = Angle(v1);
  else rotangle = 0.0;

  x0 = Rotate(x0, -rotangle);
  v0 = Rotate(v0, -rotangle);
  v1 = Rotate(v1, -rotangle);

  compute_motion_1d(x0.X, v0.X, v1.X, a_max, v_max, a_factor,
		    traj_accel.X, time_x);
  compute_motion_1d(x0.Y, v0.Y, v1.Y, a_max, v_max, a_factor,
		    traj_accel.Y, time_y);

  if (Magnitude(v1) == 0.0) {
    float rx = time_x / hypot(time_x, time_y);
    float ry = time_y / hypot(time_x, time_y);
    
    compute_motion_1d(x0.X, v0.X, v1.X, a_max * rx, v_max * rx, a_factor,
		      traj_accel.X, time_x);
    compute_motion_1d(x0.Y, v0.Y, v1.Y, a_max * ry, v_max * ry, a_factor,
		      traj_accel.Y, time_y);
  } else {
    if (time_x < time_y * 1.5) {
      float rx_a = pow(time_x / (time_y * 1.5), 2.0);
      float rx_v = time_x / (time_y * 1.5);

      compute_motion_1d(x0.X, v0.X, v1.X, a_max * rx_a, v_max * rx_v, a_factor,
			traj_accel.X, time_x);
    }
  }

  traj_accel = Rotate(traj_accel, rotangle);

  time = max(time_x, time_y);
}

float compute_stop(float v, float max_a)
{
  if (fabs(v) > max_a / 61.0) return copysign(max_a, -v);
  else return -v * 61.0;
}

Trajectory goto_point(RobotState state,
					 RobotState target,
					VelocityProfile *profile)
{
	static TVec3 oldAns[12] = {Vec3(0.0f),Vec3(0.0f),Vec3(0.0f),Vec3(0.0f),Vec3(0.0f),Vec3(0.0f),Vec3(0.0f),Vec3(0.0f),Vec3(0.0f),Vec3(0.0f),Vec3(0.0f),Vec3(0.0f)};

  const float a_max = 5000.0f;
  const float v_max = 4000.0f;

  TVec2 x = state.Position - target.Position;
	TVec2 v = Vec2(oldAns[state.vision_id].X, oldAns[state.vision_id].Y);
  float ang = NormalizeAngle(state.Angle - target.Angle);
  float ang_v = oldAns[state.vision_id].Z;

	TVec2 target_v = Vec2(target.velocity.x, target.velocity.y);

	TVec2 a;

	float ang_a,factor_a;
	float time_a, time;

  compute_motion_2d(x, v, target_v,
                    a_max,
                    v_max,
		    1.1,
		    a, time);

  /*factor_a = 0.5 - 0.1;
  do{
    factor_a += 0.1;
    compute_motion_1d(ang, ang_v, 0.0,
                      profile->max_w_acc*factor_a,
                      profile-> VDVAR(OMNI_MAX_ANG_VEL)[type],
                      DVAR(OMNI_ANG_ACCEL_FACTOR),
                      ang_a, time_a);
  }while(factor_a<1.0 && time_a>time);
  // printf("factor_a = %f\n",factor_a);*/

  v += a / 61.0;
  ang_v += ang_a / 61.0;

  if (Magnitude(v) > v_max)
    v = Normalize(v) * v_max;
  /*ang_v = bound(ang_v,
		-VDVAR(OMNI_MAX_ANG_VEL)[type], 
		VDVAR(OMNI_MAX_ANG_VEL)[type]);*/


  Trajectory t(v.X, v.Y, 0, time);

	oldAns[state.vision_id] = Vec3(v.X, v.Y, 0.0f);

  return t;
}

#if 0
Robot::Trajectory Robot::nav_to_point(World &world, int me,
				      vector2d target_pos, vector2d target_vel,
				      double target_angle,int obs_flags,
				      GotoPointType type)
{
  obstacles obs;
  ::state initial,target,goal;
  vector2d rp,rv,p,v;
  double rs,s; // relative speeds
  vector2d q,qr,obs_vel;
  vector2d ball,ball_vel;
  double a,t,tmax,qrl,rad;
  double out_x,out_y;
  int obs_id;
  int goalie_id;
  int i;

  if (!cr_setup_diff) {
    CR_SETUP(robot, NAV_THEIR_OBSTACLE_RADIUS, CR_DOUBLE);
    CR_SETUP(robot, NAV_OUR_OBSTACLE_RADIUS, CR_DOUBLE);
    CR_SETUP(robot, NAV_THEIR_GOALIE_OBSTACLE_RADIUS, CR_DOUBLE);

    cr_setup_robot = true;
  }

  gui_debug_line(me, GDBG_NAVIGATION, world.teammate_position(me),
		 target_pos, G_ARROW_FORW);
  gui_debug_line(me, GDBG_NAVIGATION, target_pos, target_pos + target_vel,
		 G_ARROW_FORW);

  rp = world.teammate_position(me);
  rv = world.teammate_velocity(me);
  rs = rv.length();
  v.set(0,0);
  tmax = closest_point_time(rp,rv,target_pos,v);
  ball     = world.ball_position();
  ball_vel = world.ball_velocity();

  // set up teammates as obstacles
  for(i=0; i<world.n_teammates; i++){
    if(i!=me && (OBS_TEAMMATE(i) & obs_flags)){
      p = world.teammate_position(i);
      v = world.teammate_velocity(i);
      s = v.length();

      // find where we would hit, where each robot is responsible only
      // for its fraction of the total speed.
      t = closest_point_time(rp,rv,p,v);
      t *= rs / (rs + s + EPSILON);

      // if we can get to our target, or its far away, don't bother
      t = min3(t,tmax,4.0);

      /*
      printf("  s=(%7.2f,%7.2f) t=%5.4f tmax=%5.4f d=%7.2f\n",
             rv.length(),v.length(),t,tmax,
             Vector::distance(rp,p+v*t));
      */
      q = p + v*t;
      obs.add_circle(q.x,q.y,DVAR(NAV_OUR_OBSTACLE_RADIUS),v.x,v.y,1);
      // printf("me=%d t=%f\n",me,t);
    }
  }

  // set up opponents as obstacles
  goalie_id = world.orole_goalie;
  if(true){
    for(i=0; i<world.n_opponents; i++){
      if(OBS_OPPONENT(i) & obs_flags){
        p = world.opponent_position(i);
        v = world.opponent_velocity(i);
        s = v.length();

        // find where we would hit, where each robot is responsible only
        // for its fraction of the total speed.
        t = closest_point_time(rp,rv,p,v);
        t *= rs / (rs + s + EPSILON);

        // if we can get to our target, or its far away, don't bother
        t = min3(t,tmax,4.0);

        q = p + v*t;
	rad = (i != goalie_id)? DVAR(NAV_THEIR_GOALIE_OBSTACLE_RADIUS) :
              DVAR(NAV_THEIR_OBSTACLE_RADIUS);
	obs.add_circle(q.x,q.y,rad,v.x,v.y,1);
      }
    }
  }else{
    for(i=0; i<world.n_opponents; i++){
      if(OBS_OPPONENT(i) & obs_flags){
        p = world.opponent_position(i);
        v = world.opponent_velocity(i);
        obs.add_circle(p.x,p.y,DVAR(NAV_THEIR_OBSTACLE_RADIUS),v.x,v.y,1);
      }
    }
  }

  // walls
  if(obs_flags & OBS_WALLS){
    obs.add_half_plane(-FIELD_LENGTH_H-GOAL_DEPTH, 0, 1, 0,1);
    obs.add_half_plane( FIELD_LENGTH_H+GOAL_DEPTH, 0,-1, 0,1);
    obs.add_half_plane( 0,-FIELD_WIDTH_H, 0, 1,1);
    obs.add_half_plane( 0, FIELD_WIDTH_H, 0,-1,1);

    obs.add_rectangle(-FIELD_LENGTH_H-GOAL_DEPTH,
                       (FIELD_WIDTH_H+GOAL_WIDTH_H)/2,
                      GOAL_DEPTH,FIELD_WIDTH_H-GOAL_WIDTH_H,1);
    obs.add_rectangle(-FIELD_LENGTH_H-GOAL_DEPTH,
                      -(FIELD_WIDTH_H+GOAL_WIDTH_H)/2,
                      GOAL_DEPTH,FIELD_WIDTH_H-GOAL_WIDTH_H,1);

    obs.add_rectangle( FIELD_LENGTH_H+GOAL_DEPTH,
                       (FIELD_WIDTH_H+GOAL_WIDTH_H)/2,
                      GOAL_DEPTH,FIELD_WIDTH_H-GOAL_WIDTH_H,1);
    obs.add_rectangle( FIELD_LENGTH_H+GOAL_DEPTH,
                      -(FIELD_WIDTH_H+GOAL_WIDTH_H)/2,
                      GOAL_DEPTH,FIELD_WIDTH_H-GOAL_WIDTH_H,1);
  }

  // defense zones
  if(obs_flags & OBS_OUR_DZONE){
    obs.add_rectangle(-FIELD_LENGTH_H-DEFENSE_DEPTH,0,
                      DEFENSE_DEPTH*4,DEFENSE_WIDTH,1);

    // make 100% sure we're not going into defense zone
    out_x = (-FIELD_LENGTH_H+DEFENSE_DEPTH+100) - target_pos.x;
    out_y = (DEFENSE_WIDTH_H+100) - fabs(target_pos.y);
    // printf("  out_x=%f out_y=%f\n",out_x,out_y);

    if(out_x>0 && out_y>0){
      if(out_x < out_y){
        target_pos.x += out_x;
      }else{
        target_pos.y += sign_nz(target_pos.y)*out_y;
      }
    }
  }

  if(obs_flags & OBS_THEIR_DZONE){
    obs.add_rectangle( FIELD_LENGTH_H+DEFENSE_DEPTH,0,
                       DEFENSE_DEPTH*4,DEFENSE_WIDTH,1);
  }

  // ball
  if(obs_flags & OBS_BALL){
    t = bound(Vector::distance(ball,rp)-180,30,60);
    obs.add_circle(ball.x,ball.y,t,ball_vel.x,ball_vel.y,1);
  }
  obs.set_mask(1);

  // set initial state
  p = world.teammate_position(me);
  v = world.teammate_velocity(me);
  a = world.teammate_direction(me);
  initial.pos = vdtof(p);
  // initial.vel = vdtof(v);
  goal.pos = vdtof(target_pos);
  // goal.vel = vdtof(target_vel);

  // plan
  obs_id = -1;
  target = world.path[me].plan(&obs,1,initial,goal,obs_id);

  if(false){
    printf("  Init(%f,%f)  Goal(%f,%f)  Target(%f,%f)\n",
           V2COMP(initial.pos),V2COMP(goal.pos),V2COMP(target.pos));
    if(target.pos.length() < 10) exit(1);
  }

  q   = vftod(target.pos);
  qr  = q - p;
  qrl = qr.length();
  qr /= qrl;

  if(Vector::distance(q,target_pos)>1 && qrl>1){
    if(obs_id >= 0){
      obs_vel.set(obs.obs[obs_id].vel.x,obs.obs[obs_id].vel.y);
      s = bound(Vector::dot(qr,obs_vel)+500.0,0.0,1500.0);
    }else{
      s = 500;
    }
    v = qr * s;
  }else{
    v = target_vel;
  }

  return goto_point(world, me, q, v, target_angle, type);
}
#endif
