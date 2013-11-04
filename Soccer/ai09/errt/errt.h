#pragma once

#include "tree.h"
#include "obstacle.h"
#include <limits.h>


/*#ifndef INT_MAX
#define INT_MAX 2147483647 //2,147,483,647
#endif*/

//float rnd ( void );

class Planner
{
	float step_size;
	TVec2 init_state , final_state;
	TVec2 waypoint[MAX_NODES];
        unsigned int waypoints , cached_waypoints , cache_start;

	void reverse_waypoints ( void );

public:

	float goal_target_prob;
	float waypoint_target_prob;
	float acceptable_dis;

	Planner ( void );
	Tree tree;
	void init ( TVec2 init , TVec2 final , float step );
        TVec2 random_state ( void );
	TVec2 nearest_free ( TVec2 state );
        TVec2 choose_target ( int * type = NULL );
	Node * extend ( Node * s , TVec2 & target );
	void SetWayPoints ( void );
	TVec2 GetWayPoint ( unsigned int i );
	unsigned int GetWayPointNum ( void );
	bool IsReached ( void );

	void optimize_tree ( void );

	TVec2 Plan ( void );
};
