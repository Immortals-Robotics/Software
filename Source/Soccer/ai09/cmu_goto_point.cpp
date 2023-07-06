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
#include "errt/errt.h"
#include "ai09.h"


// Calculates the timings for a trapezoidal velocity profile with
// constant acceleration.  'dx' is the distance to be traveled, and
// 'vel0' and 'vel1' are the desired initial and final velocities.
// 'max_vel' == maximum veloicty, 'max_accel' == maximum acceleration
// the individual timings are returned in the last three parameters:
// 't_accel' is the time spent in the acceleration phase, 't_cruise'
// is the time spent at maximum velocity, and 't_decel' is the time
// spent in the deceleration phase.  Any of these can be 0.  The total
// of the three times is returned by the function.
float motion_time_1d(float dx, float vel0, float vel1,
                     float max_vel, float max_accel,
                     float &t_accel, float &t_cruise, float &t_decel)
{
    float tmp, vmax;

    if (dx < 0) {
        dx = -dx;
        vel0 = -vel0;
        vel1 = -vel1;
    }

    if (vel0 > max_vel) vel0 = max_vel;
    if (vel1 > max_vel) vel1 = max_vel;

    // stop
    if (vel0 > vel1) {
        t_decel = (vel0 + vel1) / 2 * dx;
        if (fabs(vel1 - vel0) / t_decel > max_accel) {
            t_accel = t_cruise = 0;
            return(t_decel);
        }
    }

    // calculate time spent at max velocity
    tmp = 2 * max_accel*dx + sq(vel0) + sq(vel1);
    t_cruise = (tmp - 2 * sq(max_vel)) / (2 * max_vel*max_accel);

    if (t_cruise > 0) {
        vmax = max_vel;
    }
    else {
        vmax = sqrt((max_accel*dx + sq(vel0) + sq(vel1)) / 2);
        t_cruise = 0;
    }

    t_accel = max(vmax - vel0, 0.0f) / max_accel;
    t_decel = fabs(vmax - vel1) / max_accel;

    return(t_accel + t_cruise + t_decel);
}

// maximum speed we will be willing to go if we are dx away from
// target and have an acceleration of max_a
double max_speed(float dx, float max_a)
{
    return(sqrt(2 * max_a*dx));
}

void compute_motion_1d(float x0, float v0, float v1,
                       float a_max, float v_max, float a_factor,
                       float &traj_accel, float &traj_time)
{
    // First check to see if nothing needs to be done...
    if (x0 == 0 && v0 == v1) { traj_accel = 0; traj_time = 0;  return; }

    if (!isfinite(x0) || !isfinite(v0) || !isfinite(v1)) {
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
            traj_accel = time_to_stop / period * -copysign(a_max * a_factor, v0) +
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
    }
    else if (x_to_v1 > fabs(x0)) {
        t_a = (sqrt(v0 * v0 + 2 * a_max * fabs(x0)) - fabs(v0)) / a_max;
        t_accel = t_a;
        t_decel = 0.0;
    }
    else {
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
    if (a_to_v1_at_x0 < a_max && a_to_v1_at_x0 > 0.0 &&
        t_to_v1_at_x0 < 2.0 / 61.0) {

        // OPTION 2
        // Use option 1 time, even though we're not following it.
        traj_time = t_accel + t_decel;;

        // Target acceleration to stop at x0.
        traj_accel = -copysign(a_to_v1_at_x0, v0);

        return;
    }
    else {

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
        }
        else if (t_accel < period && t_decel > 0.0) {
            traj_accel = compute_stop(v0, a_max * a_factor);
        }
        else if (t_accel < period) {
            traj_accel = copysign((2.0 * t_accel / (period)-1) * a_max * a_factor, v0);
        }
        else {
            traj_accel = copysign(a_max * a_factor, -x0);
        }
    }
}

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
//        std::cout<<"STEP111"<<std::endl;
    }
    else {
//        if (time_x < time_y * 1.5) {
//            float rx_a = pow(time_x / (time_y * 1.5), 2.0);
//            float rx_v = time_x / (time_y * 1.5);
//
//            compute_motion_1d(x0.X, v0.X, v1.X, a_max * rx_a, v_max * rx_v, a_factor,
//                              traj_accel.X, time_x);
//            std::cout<<"STEP222"<<std::endl;
//        }
//        std::cout<<"STEP333"<<std::endl;
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
    static TVec3 oldAns[12] = { Vec3(0.0f),Vec3(0.0f),Vec3(0.0f),Vec3(0.0f),Vec3(0.0f),Vec3(0.0f),Vec3(0.0f),Vec3(0.0f),Vec3(0.0f),Vec3(0.0f),Vec3(0.0f),Vec3(0.0f) };

    const float a_max = profile->max_acc.X;
    const float v_max = profile->max_spd.X;// * (target.velocity.x/100.0f);//TODO added this for a better use of speed
    const float ang_a_max = 700.0;
    const float ang_v_max = 300.0;

    TVec2 x = state.Position - target.Position;
    TVec2 v = Vec2(oldAns[state.vision_id].X, oldAns[state.vision_id].Y);
    float ang = NormalizeAngle(state.Angle - target.Angle);
    float ang_v = oldAns[state.vision_id].Z;

    TVec2 target_v = Vec2(target.velocity.x, target.velocity.y);

    TVec2 a;

    float ang_a, factor_a;
    float time_a, time;

    compute_motion_2d(x, v, target_v,
                      a_max,
                      v_max,
                      1.1,
                      a, time);

    factor_a = 0.5 - 0.1;
    do {
        factor_a += 0.1;
        compute_motion_1d(ang, ang_v, 0.0,
                          ang_a_max*factor_a,
                          ang_v_max,
                          1.1,
                          ang_a, time_a);
    } while (factor_a<1.0 && time_a>time);
    // printf("factor_a = %f\n",factor_a);

    v += a/61.0;
    ang_v += ang_a / 61.0;

    if (Magnitude(v) > v_max)
        v = Normalize(v) * v_max;
    ang_v = min(ang_v_max, max(-ang_v_max, ang_v));

    Trajectory t(v.X, v.Y, ang_v, time);

    oldAns[state.vision_id] = Vec3(v.X, v.Y, ang_v);

    return t;
}

Trajectory nav_to_point(
        RobotState state,
        RobotState target,
        VelocityProfile *profile,
        Planner *planner)
{
    TVec2 rp, rv, p, v;
    float rs, s; // relative speeds
    TVec2 q, qr, obs_vel;
    float a, t, tmax, qrl, rad;
    float out_x, out_y;
    int obs_id;
    int goalie_id;
    int i;

    rp = state.Position;
    rv = Vec2(state.velocity.x, state.velocity.y);
    rs = Magnitude(rv);
    v = Vec2(0);



    // set initial state
    p = state.Position;
    v = Vec2(state.velocity.x, state.velocity.y);
    a = state.velocity.direction;

    planner->init(state.Position, target.Position, 90.0f);
    TVec2 wayp = planner->Plan();

    q = wayp;
    qr = q - p;
    qrl = Magnitude(qr);
    qr /= qrl;

    if (DIS(q, target.Position) > 1 && qrl > 1) {
        v = qr * 500.0;
    }
    else {
        v = Vec2(target.velocity.x, target.velocity.y);
    }

    return Trajectory();//goto_point(world, me, q, v, target_angle, type);
}
