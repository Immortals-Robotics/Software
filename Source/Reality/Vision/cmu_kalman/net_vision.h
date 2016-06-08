// net_vision.h
//
// Network interface for vision socket.  See socket.h for info
// on connecting a client. 
//
// Given a connected client, here's some example code...
//
// net_vframe frame;
//
// while(client->ready_for_read()) {
//   client->recv(&frame, sizeof(frame));
//   ...
// }
// ...
// vref ref = { NET_VISION_REF, REF_STOP };
// client->send(&ref, sizeof(ref));
// ...
//
// Created by:  Michael Bowling (mhb@cs.cmu.edu)
//              Adapted from CMDragons 2001 (jbruce@cs.cmu.edu)
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

#ifndef __net_vision_h__
#define __net_vision_h__

#include "constants.h"
#include "vector.h"
#include "macros.h"
#include "geometry.h"

struct net_vframe;
struct net_vconfig;
struct net_vref;

struct vconfig_robot {
  char id;             // The robot's id
}; // 2

#define VCOVER_NONE   0
#define VCOVER_NORMAL 1

struct vconfig_team {
  char cover_type;
  vconfig_robot robots[MAX_TEAM_ROBOTS];
}; // 1+5*2 = 11

struct net_vconfig {
  char msgtype; // = NET_VISION_CONFIG
  vconfig_team teams[NUM_TEAMS];
}; // 1+11*2 = 23

//
// vref
// 

#define REF_STOP    0
#define REF_GO      1

struct net_vref {
  char msgtype; // = NET_VISION_REF
  char refstate;
};

//
// vsim
//

#define VSIM_MOVEBALL  1
#define VSIM_MOVEROBOT 2

struct vsim_moveball {
  vector2d_struct pos;
  vector2d_struct vel;
};

struct vsim_moverobot {
  char team, array_id;
  vector2d_struct pos;
  double angle;
};

struct net_vsim {
  char msgtype; // = NET_VISION_SIM
  char command; // = VSIM_*

  union {
    vsim_moveball moveball;
    vsim_moverobot moverobot;
  } info;
};

const int net_vision_in_maxsize = MAX(sizeof(net_vconfig), 
				  MAX(sizeof(net_vref), 
				      sizeof(net_vsim)));

// ------------------------------------------------------------------
// Output Messages
// ------------------------------------------------------------------

#define NET_VISION_FRAME   1

struct net_vframe;

struct vraw {
  double timestamp;
  vector2f pos;
  float angle;
  float conf;
}; // 8+4*2+4*2 = 24

struct vball {
  struct { float x; float y; float vx; float vy; } state;
  float variances[16];

  vraw vision;

  vector2f occluding_offset;
  char occluded;
  char occluding_team, occluding_robot;
}; // 20*4 + 24 + 8 + 3 = 115

struct vrobot {
  struct { 
    float x; float y; float theta; 
    float vx; float vy; float vtheta;
    float stuck; } state;
  vraw vision;
}; // 6*4 + 24 = 48

struct net_vframe {
  char msgtype;

  double timestamp;

  vball ball;
  vrobot robots[NUM_TEAMS][MAX_TEAM_ROBOTS];

  char refstate;

  net_vconfig config;
}; // 8+8+104+48*2*5+8+23 = 631

const int net_vision_out_maxsize = sizeof(net_vframe);

#endif
