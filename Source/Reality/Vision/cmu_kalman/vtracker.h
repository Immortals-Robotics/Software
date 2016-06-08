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

#ifndef VTRACKER_H
#define VTRACKER_H

#include "constants.h"
#include "net_vision.h"
#include "ball_tracker.h"
#include "robot_tracker.h"

#include <stdio.h>
#include <stdlib.h>

class VTracker {
public:
  BallTracker ball;
  RobotTracker robots[NUM_TEAMS][MAX_TEAM_ROBOTS];

    // temp matrix to store ball covariances
  Matrix bcovar;

public:
  VTracker(void);

  double Height(int team, int robot) { return OMNIBOT_HEIGHT; }
  double Radius(int team, int robot) { return 90.0; }

  void ResetAll(void);

  void GetBallData(vball &vb, double dt);
  void GetRobotData(vrobot &vr, int team, int indx, double dt);
};

#endif /* __VTRACKER_H__ */


