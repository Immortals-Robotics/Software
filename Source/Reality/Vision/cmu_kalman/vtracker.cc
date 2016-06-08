/*
 * TITLE:    Vtracker.cc
 *
 * PURPOSE:  Encapsulates the Extended Kalman Filter Tracker system
 *           It wraps around the ball_Tracker and robot_tracker classes
 *           It needs to be wrapped in a single class as there are 
 *           between tracking objects interdependencies
 *          
 * WRITTEN BY: Michael Bowling, Brett Browining
 */
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

#include "vtracker.h"
#include <string.h>
#include <assert.h>
#include <stdio.h>

/****************************** CONSTANTS ****************************/

#define ISBLUE(c)          ((c) == TEAM_BLUE)
#define CHOOSETEAM(c, b, y)   (ISBLUE(c) ? (b) : (y))

//#define DEBUG

/****************************** CODE *********************************/

VTracker::VTracker(void)
{
#ifdef DEBUG
  fprintf(stderr, "SIMTRACKER:constructor\n");
#endif

  for (int t = 0; t < NUM_TEAMS; t++) {
    for (int i = 0; i < MAX_TEAM_ROBOTS; i++) {
      index2id[t][i] = -1;
    }
    for (int i = 0; i < MAX_ROBOT_ID; i++)
      id2index[t][i] = -1;
  }

  ball.set_tracker(this);
}
    
  
  // set the configuration for the EKBF's 
void VTracker::SetConfig(const net_vconfig &vcfg)
{
#ifdef DEBUG
  fprintf(stderr, "SIMTRACKER:Setting config\n");
#endif

  vconfig = vcfg;

  // clear out idnexes and types
  for (int t = 0; t < NUM_TEAMS; t++) {
    for (int i = 0; i < MAX_TEAM_ROBOTS; i++) {
      index2id[t][i] = -1;
    }
    for (int i = 0; i < MAX_ROBOT_ID; i++)
      id2index[t][i] = -1;
  }

  // set it all
  for (int t = 0; t < NUM_TEAMS; t++) {
    for (int i = 0; i < MAX_TEAM_ROBOTS; i++) {
      if (vcfg.teams[t].robots[i].id >= 0) {
#ifdef DEBUG
	fprintf(stderr, "Setting team %i, ind %i, id %i\n", t, i, vcfg.teams[t].robots[i].id);
#endif

	index2id[t][i] = vcfg.teams[t].robots[i].id;
	id2index[t][vcfg.teams[t].robots[i].id] = i;
      }
    }
  }
}  

void VTracker::ResetAll(void) 
{
  ball.reset();
  for (int t = 0; t < NUM_TEAMS; t++) {
    for (int i = 0; i < MAX_TEAM_ROBOTS; i++) {
      if (index2id[t][i] >= 0) 
	robots[t][i].reset();
    }
  }
}
  
void VTracker::GetBallData(vball &vb, double dt)
{
  // fill out tracking info
  bcovar = ball.covariances(dt);
  bcovar.CopyData(vb.variances);

  vector2d tmp = ball.position(dt);
  vb.state.x = (float) tmp.x;
  vb.state.y = (float) tmp.y;
  tmp = ball.velocity(dt);
  vb.state.vx = (float) tmp.x;
  vb.state.vy = (float) tmp.y;

  vb.occluded = ball.occluded;
  vb.occluding_team = ball.occluding_team;
  vb.occluding_robot = ball.occluding_robot;
  vb.occluding_offset = vector2f(ball.occluding_offset.x, ball.occluding_offset.y);
}

void VTracker::GetRobotData(vrobot &vr, int team, int indx, double dt)
{
  // Get the state information from the Kalman filter
  vector2d tmp = robots[team][indx].position(dt);
  vr.state.x = (float) tmp.x;
  vr.state.y = (float) tmp.y;
  vr.state.theta = robots[team][indx].direction(dt);
  
  tmp = robots[team][indx].velocity_raw(dt);
  vr.state.vx = (float) tmp.x;
  vr.state.vy = (float) tmp.y;
  vr.state.vtheta = robots[team][indx].angular_velocity(dt);

  vr.state.stuck = robots[team][indx].stuck(dt);
}
