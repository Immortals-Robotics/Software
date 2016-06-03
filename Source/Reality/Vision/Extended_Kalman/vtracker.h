/*
 * TITLE:    VTracker.h
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

#ifndef __VTRACKER_H__
#define __VTRACKER_H__

#include <constants.h>
#include <reality/net_vision.h>
#include <ball_tracker.h>
#include <robot_tracker.h>

#include <stdio.h>
#include <stdlib.h>

class VTracker {
public:
  BallTracker ball;
  RobotTracker robots[NUM_TEAMS][MAX_TEAM_ROBOTS];
  int id2index[NUM_TEAMS][MAX_ROBOT_ID];
  int index2id[NUM_TEAMS][MAX_TEAM_ROBOTS];

  net_vconfig vconfig;

  // temp matrix to store ball covariances
  Matrix bcovar;

public:
  VTracker(void);

  bool Exists(int team, int robot) { return (index2id[team][robot] >= 0); }
  double Height(int team, int robot) { 
    switch(Type(team, robot)) {
    case ROBOT_TYPE_DIFF: return DIFFBOT_HEIGHT;
    case ROBOT_TYPE_OMNI: return OMNIBOT_HEIGHT;
    default: return OPPONENT_HEIGHT;
    }}

  double Radius(int team, int robot) { return 90.0; }

  int Type(int team, int robot) { 
    if (vconfig.teams[team].cover_type == VCOVER_NORMAL) 
      return vconfig.teams[team].robots[robot].type;
    else
      return ROBOT_TYPE_NONE; }

    
  // set the configuration for the EKBF's 
  void SetConfig(const net_vconfig &vcfg);
  void ResetAll(void);

  void GetBallData(vball &vb, double dt);
  void GetRobotData(vrobot &vr, int team, int indx, double dt);
};

#endif /* __VTRACKER_H__ */


