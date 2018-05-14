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

#include "helpers.h"#include "Robot.h"
#include "VelocityProfile.h"
#include "../../Reality/WorldState.h"

struct Trajectory
{
    double vx, vy, va;

    double eta;

    Trajectory(){;}

    Trajectory(double _vx, double _vy, double _va,
               double _eta = 0.0)
    {
      vx = _vx; vy = _vy; va = _va;
      eta = _eta;
    }
};

float motion_time_1d(float dx,float vel0,float vel1,
                            float max_vel,float max_accel,
                            float &t_accel,float &t_cruise,float &t_decel);

double max_speed(float dx,float max_a);
void compute_motion_1d(float x0, float v0, float v1,
                       float a_max, float v_max, float a_factor,
                       float &traj_accel, float &traj_time);

void compute_motion_2d(TVec2 x0, TVec2 v0, TVec2 v1,
                       float a_max, float v_max, float a_factor,
                              TVec2 &traj_accel, float &time);

float compute_stop(float v, float max_a);

Trajectory goto_point(RobotState state,
					  RobotState target,
					  VelocityProfile *profile);
// 1400.0, 2000.0, 12.0, 12.0
// 1600.0, 3000.0, 18.0, 18.0

// Normal
const float diff_max_vx  = 1200.0; // 1600.0;
const float diff_max_avx = 2000.0; // 3000.0;
const float diff_max_va  =   14.0; //   18.0;
const float diff_max_ava =   14.0; //   18.0;
const float diff_accel_margin = 0.85;



Trajectory goto_point_speed(int me,
                                     TVec2 target_pos,TVec2 target_vel,
                                     double target_angle);
