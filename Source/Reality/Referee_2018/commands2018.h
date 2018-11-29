/*
 * TITLE:	Commands.h
 *
 * PURPOSE: This file contains the constants that define the various 
 *			serial commands the referee box can send to the operating
 *			computers.
 *
 * WRITTEN BY: Brett Browning
 */
/* LICENSE:  =========================================================================
    RoboCup F180 Referee Box Source Code Release
  -------------------------------------------------------------------------
    Copyright (C) 2003 RoboCup Federation
  -------------------------------------------------------------------------
    This software is distributed under the GNU General Public License,
    version 2.  If you do not have a copy of this licence, visit
    www.gnu.org, or write: Free Software Foundation, 59 Temple Place,
    Suite 330 Boston, MA 02111-1307 USA.  This program is distributed
    in the hope that it will be useful, but WITHOUT ANY WARRANTY,
    including MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  ------------------------------------------------------------------------- 

 */

#ifndef __COMMANDS_H__
#define __COMMANDS_H__

#include "../../Network/Protobuf/referee2018.pb.h"



// play commands
#define COMM_HALT	       SSL_Referee_Command_HALT
#define COMM_STOP		   SSL_Referee_Command_STOP
#define COMM_NORMAL_START       SSL_Referee_Command_NORMAL_START
#define COMM_FORCE_START		SSL_Referee_Command_FORCE_START
//#define COMM_READY	           ' '

// game flow commands
#define COMM_FIRST_HALF		   '1'
#define COMM_HALF_TIME		   'h'
#define COMM_SECOND_HALF	   '2'
#define COMM_OVER_TIME1		   'o'
#define COMM_OVER_TIME2		   'O'
#define COMM_PENALTY_SHOOTOUT	   'a'

/* timeout commands */
#define COMM_TIMEOUT_YELLOW	   SSL_Referee_Command_TIMEOUT_YELLOW
#define COMM_TIMEOUT_BLUE	   SSL_Referee_Command_TIMEOUT_BLUE
#define COMM_TIMEOUT_END           'z'

#define COMM_CANCEL                'c'

// goal status
#define COMM_GOAL_YELLOW	   SSL_Referee_Command_GOAL_YELLOW
#define COMM_GOAL_BLUE		   SSL_Referee_Command_GOAL_BLUE
#define COMM_SUBGOAL_YELLOW	   'd'
#define COMM_SUBGOAL_BLUE	   'D'

// penalty signals
#define COMM_YELLOWCARD_YELLOW 'y'
#define COMM_YELLOWCARD_BLUE   'Y'

#define COMM_REDCARD_YELLOW    'r'
#define COMM_REDCARD_BLUE      'R'


/* game flow commands */
#define COMM_RESTART		  'n'

#define COMM_KICKOFF_YELLOW	  SSL_Referee_Command_PREPARE_KICKOFF_YELLOW
#define COMM_KICKOFF_BLUE	  SSL_Referee_Command_PREPARE_KICKOFF_BLUE

#define COMM_PENALTY_YELLOW	  SSL_Referee_Command_PREPARE_PENALTY_YELLOW
#define COMM_PENALTY_BLUE	  SSL_Referee_Command_PREPARE_PENALTY_BLUE

#define COMM_DIRECT_YELLOW	  SSL_Referee_Command_DIRECT_FREE_YELLOW
#define COMM_DIRECT_BLUE      SSL_Referee_Command_DIRECT_FREE_BLUE

#define COMM_INDIRECT_YELLOW  SSL_Referee_Command_INDIRECT_FREE_YELLOW
#define COMM_INDIRECT_BLUE	  SSL_Referee_Command_INDIRECT_FREE_BLUE


/*ball placement*/
#define COMM_PLACEBALL_YELLOW SSL_Referee_Command_BALL_PLACEMENT_YELLOW
#define COMM_PLACEBALL_BLUE SSL_Referee_Command_BALL_PLACEMENT_BLUE


// acceptable referee commands
#define COMM_CMD_STRING      "iIfFpPkKnrRyYdDgGcztTaoO2h1 HsS"

#endif /* __COMMANDS_H__ */

