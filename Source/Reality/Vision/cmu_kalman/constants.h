/*
 * TITLE:        constants.h
 *
 * PURPOSE:      This is file contains the major system constants
 *               
 * WRITTEN BY:   Michael Bowling, James R Bruce, Brett Browning
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

#ifndef __CONSTANTS_H__
#define __CONSTANTS_H__

#define MAX_ROBOTS      10
#define MAX_TEAM_ROBOTS  5

#define MAX_ROBOT_ID    16
#define MAX_ROBOT_ID_OURS   10

#define TEAM_BLUE   0
#define TEAM_YELLOW 1
#define TEAM_NONE   2
#define NUM_TEAMS   2

#define SIDE_LEFT   1
#define SIDE_RIGHT -1

/* robot types */
#define ROBOT_TYPE_NONE   0
#define ROBOT_TYPE_DIFF   1
#define ROBOT_TYPE_OMNI   2
#define ROBOT_TYPE_GOALIE 3
#define NUM_ROBOT_TYPES   4

// check these!
#define DIFF_WHEELBASE_RADIUS 60.0
#define OMNI_WHEELBASE_RADIUS 60.0
/* OMNI_WHEELBASE_RADIUS is 73.0 from measurement */

/* macro for team identity */
#define IS_YELLOW(y, a, b)     ((y) ? (a) : (b))

/* macro to determine the robot type from the id */
#define ROBOT_TYPE_ID(id)      (((id) > 4) ? ROBOT_TYPE_OMNI : ROBOT_TYPE_DIFF)


//==== Field Dimensions (mm) =========================================//

// diagonal is of 2800 x 2300 is 3623.53
#define FIELD_LENGTH    2800 /* 2780 */
#define FIELD_WIDTH     2400 /* 2300 1525 */
#define FIELD_LENGTH_H  (FIELD_LENGTH /2)
#define FIELD_WIDTH_H   (FIELD_WIDTH  /2)

#define GOAL_WIDTH          600
#define GOAL_DEPTH          180
#define DEFENSE_WIDTH      1000
#define DEFENSE_DEPTH       225
#define WALL_WIDTH           50
#define CORNER_BLOCK_WIDTH   40

#define GOAL_WIDTH_H     (GOAL_WIDTH   /2)
#define GOAL_DEPTH_H     (GOAL_DEPTH   /2)
#define DEFENSE_WIDTH_H  (DEFENSE_WIDTH/2)
#define DEFENSE_DEPTH_H  (DEFENSE_DEPTH/2)

#define PENALTY_FROM_GOAL   (225)
#define PENALTY_SPOT        (FIELD_LENGTH_H - PENALTY_FROM_GOAL)

#define CENTER_CIRCLE_RADIUS (460/2)

#define FREEKICK_FROM_WALL 150
#define FREEKICK_FROM_GOAL 375

#define BALL_RADIUS      21
#define BALL_DIAMETER    (BALL_RADIUS * 2)

//==== Robot Dimensions (mm) =========================================//

#define ROBOT_DEF_WIDTH     180.0
#define ROBOT_DEF_WIDTH_H   (ROBOT_DEF_WIDTH / 2.0)

#define DIFFBOT_WIDTH       160.0
#define DIFFBOT_LENGTH      120.0
#define DIFFBOT_WIDTH_H     (DIFFBOT_WIDTH / 2.0)
#define DIFFBOT_LENGTH_H    (DIFFBOT_LENGTH / 2.0)
#define DIFFBOT_HEIGHT      100.0
#define DIFFBOT_RADIUS      100.0 // sqrt(width^2 + length^2)

#define OMNIBOT_RADIUS       90.0
#define OMNIBOT_HEIGHT      150.0

#define TEAMMATE_HEIGHT     100.0
#define OPPONENT_HEIGHT     100.0

#define TEAMMATE_EFFECTIVE_RADIUS  100.0
#define OPPONENT_EFFECTIVE_RADIUS  90.0


//==== Obstacle Flags ================================================//

// Standard Obstacles
#define OBS_BALL         (1 << 0)
#define OBS_WALLS        (1 << 1)
#define OBS_THEIR_DZONE  (1 << 2)
#define OBS_OUR_DZONE    (1 << 3)
#define OBS_TEAMMATE(id) (1 << (4 + id))
#define OBS_OPPONENT(id) (1 << (4 + MAX_TEAM_ROBOTS + id))
#define OBS_TEAMMATES    ( ((1 << MAX_TEAM_ROBOTS) - 1) << 4)
#define OBS_OPPONENTS    ( ((1 << MAX_TEAM_ROBOTS) - 1) << 4 + MAX_TEAM_ROBOTS)

#define OBS_EVERYTHING (~ ((int) 0))
#define OBS_EVERYTHING_BUT_ME(id) (OBS_EVERYTHING & (~(OBS_TEAMMATE(id))))
#define OBS_EVERYTHING_BUT_US (OBS_EVERYTHING & (~(OBS_TEAMMATES)))
#define OBS_EVERYTHING_BUT_BALL (OBS_EVERYTHING & (~(OBS_BALL)))

//==== Miscellaneous =================================================//

/* Frame time and latencies */
#define FRAME_RATE    30.0
#define FRAME_PERIOD  (1.0 / FRAME_RATE)

// this needs to be remeasured
#define LATENCY_DELAY 0.100

#define CAMERA_HEIGHT 3000.0

/* Constants */
#define GRAVITY 9800 // mm/s^2

#ifndef M_2PI
#define M_2PI     6.28318530717958648
#endif
#ifndef M_PI_8
#define M_PI_8    0.39269908169872414
#endif
#ifndef M_PI_16
#define M_PI_16   0.19634954084936207
#endif

/* Macros to convert between radians and degrees */
#define RAD2DEG(r)	((r) * 180.0 / M_PI)
#define DEG2RAD(r)	((r) * M_PI / 180.0)

//==== Network Info ===================================================//

// this will change - BB
#define PORT_MAIN              32882
#define PORT_VISION_SERVER     32883
#define PORT_VISION_SERVER_OUT (PORT_VISION_SERVER + 1)
#define PORT_RADIO_SERVER      32885
#define PORT_RADIO_SERVER_OUT  (PORT_RADIO_SERVER + 1)

//==== Polygon Info ===================================================//

// diff drive
#define POLY_DIFF (Polygon(6, \
	 (const double []) {60,  60, -35, -60, -60, -35}, \
	 (const double []) {75, -75, -75, -50,  50,  75}))
#define POLY_OMNI (Polygon(6, \
         (const double []) { 60.6,  60.6,  25.98, -86.6, -86.6,  25.98}, \
         (const double []) { 65.0, -65.0, -85.0 ,  -2.0,   2.0,  85.0}))

const int poly_diff_pts = 6;
static const double poly_diff_x[poly_diff_pts] = {60,  60, -35, -60, -60, -35};
static const double poly_diff_y[poly_diff_pts] = {75, -75, -75, -50,  50,  75};

const int poly_omni_pts = 6;
static const double poly_omni_x[poly_omni_pts] = { 60.6,  60.6,  25.98, -86.6, -86.6,  25.98};
static const double poly_omni_y[poly_omni_pts] = { 65.0, -65.0, -85.0 ,  -2.0,   2.0,  85.0 };

#endif
