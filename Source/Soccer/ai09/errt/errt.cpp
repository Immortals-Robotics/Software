#include "errt.h"
#include <stdlib.h>
#include "../helpers.h"
#include "../../../Common/distance.h"

#define map2worldX(a) ((a-364)*10)
#define map2worldY(a) ((a-264)*10)

#define world2mapX(a) min(729,max(0,((a/10)+364)))
#define world2mapY(a) min(529,max(0,((a/10)+264)))

Planner::Planner ( void )
{
        waypoints = 0;
        cached_waypoints = 0;
        cache_start = 0;

        goal_target_prob = 0.1;
        waypoint_target_prob = 0.5;
        acceptable_dis = 90.0;
    
    field_width = 0;
    field_height = 0;
}

Planner::~Planner()
{
}

void Planner::init ( TVec2 init , TVec2 final , float step )
{

	init_state = nearest_free ( init );
	final_state = nearest_free ( final );
	step_size = step;

	tree.reset ( );
	tree.AddNode ( init_state , NULL );
    
    started_in_obs = IsInObstacle(init);

        /*init_state = init;
        final_state = nearest_free ( nearest_free ( final ) );
        step_size = step;

        tree.reset ( );
        tree.AddNode ( init_state , NULL );

        if ( IsInObstacle ( init_state ) )
        {
                init = nearest_free ( nearest_free ( init ) );
                tree.AddNode ( init , tree.GetNode ( 0 ) );
        }*/
}

void Planner::set_field_params ( float _w , float _h )
{
    field_width = _w;
    field_height = _h;
}

TVec2 Planner::random_state ( void )
{
        //return Vec2 ( ( rnd ( ) * 10000.0 ) - 5000.0 , ( rnd ( ) * 10000.0 ) - 5000.0 );
        return Vec2 ( ( (random.get() - 0.5f) * 2.0f * (field_width+250.0f) ) , ( (random.get() - 0.5f) * 2.0f * (field_height+250.0f) ) );
}

TVec2 Planner::nearest_free ( TVec2 state )
{
    return nearest_free_prob(state);
	int r = 678 , l = -678 , u = 478 , d = -478;

	state.X = min ( 678 , max ( 52 , state.X ) ) ;
	state.Y = min ( 478 , max ( 52 , state.Y ) ) ;
	
	if ( ! IsInObstacle ( state ) )
		return state;

	for ( int i = state.X ; i < 678 ; i ++ )
	{
		if ( ! IsInObstacle ( Vec2 ( i , state.Y ) ) )
		{
			r = i - state.X;
			break;
		}
	}

	for ( int i = state.X ; i > 52 ; i -- )
	{
		if ( ! IsInObstacle ( Vec2 ( i , state.Y ) ) )
		{
			l = i - state.X;
			break;
		}
	}

	for ( int i = state.Y ; i < 478 ; i ++ )
	{
		if ( ! IsInObstacle ( Vec2 ( state.X , i ) ) )
		{
			u = i - state.Y;
			break;
		}
	}

	for ( int i = state.Y ; i > 52 ; i -- )
	{
		if ( ! IsInObstacle ( Vec2 ( state.X , i ) ) )
		{
			d = i - state.Y;
			break;
		}
	}

	int x , y;

	if ( abs ( r ) < abs ( l ) )
		x = r;
	else
		x = l;

	if ( abs ( u ) < abs ( d ) )
		y = u;
	else
		y = d;

	//TVec2 ans = Vec2 ( min ( 729 , max ( 0 , state.X + sgn ( x ) * abs ( y ) ) ) , min ( 529 , max ( 0 , state.Y + sgn ( y ) * abs ( x ) ) ) );
	TVec2 ans = Vec2 ( state.X + sgn ( x ) * abs ( y ) , state.Y + sgn ( y ) * abs ( x ) ) ;

	//if ( IsInObstacle ( ans ) )
	//	return nearest_free ( ans );

	float coss , sinn;
	coss = ( state.X - ans.X ) / DIS ( ans , state );
	sinn = ( state.Y - ans.Y ) / DIS ( ans , state );

	TVec2 current = ans;

	while ( ( DIS ( current , state ) > 2 ) && ( ! IsInObstacle ( current ) ) )
	{
		ans = current;

		current.X += coss * 2.0f;
		current.Y += sinn * 2.0f;
	}

	return ans;
}

TVec2 Planner::nearest_free_prob ( TVec2 state )
{
    const float acceptable_free_dis = 50;
    if ( ! IsInObstacle ( state ) )
		return state;
    
    TVec2 ans = state;
    float minDis = (field_width+field_height)*10.0f;
    
    for ( int i = 0 ; i < 1000 ; i ++ )
    {
        TVec2 newRndPoint = random_state();
        if ( (!IsInObstacle(newRndPoint)) && DIS(state, newRndPoint) < minDis )
        {
            ans = newRndPoint;
            minDis = DIS(state, newRndPoint);
            if (minDis < acceptable_free_dis)
                break;
        }
    }
    
    return ans;
}

TVec2 Planner::choose_target ( int * type )
{
        float r = random.get( );

        if ( type == NULL )
            type = new int;

        if ( r <= goal_target_prob )
        {
            *type = 0;
            return final_state;
        }
        else if ( ( r <= goal_target_prob + waypoint_target_prob ) && ( cached_waypoints > 0 ) && ( cached_waypoints > cache_start ) )
        {
            r = random.get( );
            *type = (int)( r * ( cached_waypoints ) );
            if ( *type >= cached_waypoints )
                *type >= cached_waypoints;
            return waypoint[ *type ];
        }

        *type = -1;
        return random_state ( );
}

Node * Planner::extend ( Node * s , TVec2 & target )
{
        TVec2 new_state;
        float dx , dy , dis;
        dis = DIS ( s->state , target );

        if ( dis < step_size )
                return NULL;

        //dx = min ( step_size , dis ) * ( target.X - s->state.X ) / dis;
        //dy = min ( step_size , dis ) * ( target.Y - s->state.Y ) / dis;

        dx = step_size * ( target.X - s->state.X ) / dis;
        dy = step_size * ( target.Y - s->state.Y ) / dis;

        new_state.X = s->state.X + dx;
        new_state.Y = s->state.Y + dy;

        //Boundary check
        //if ( ( new_state.X >= 605 ) || ( new_state.X < 0 ) || ( new_state.Y >= 405 ) || ( new_state.Y < 0 ) )
        //        return NULL;

        //collision check
        if ( IsInObstacle ( new_state ) )
                return NULL;

        if ( collisionDetect ( new_state , s -> state ) )
                return NULL;

        return tree.AddNode ( new_state , s );
}

void Planner::SetWayPoints ( void )
{
        waypoints = 1;
        Node * n = tree.NearestNeighbour ( final_state );
        waypoint[0] = n->state;

        while ( n->parent )
        {
                n = n->parent;
                waypoint[waypoints++] = n->state;
        }

        if ( IsReached ( ) )
                cached_waypoints = waypoints;
        else
                cached_waypoints = 0;
        cache_start = 0;
}

void Planner::reverse_waypoints ( void )
{
        TVec2 tmp;
        for ( int i = 0 ; i < waypoints / 2 ; i ++ )
        {
                tmp = waypoint[i];
                waypoint[i] = waypoint[waypoints - i - 1];
                waypoint[waypoints - i - 1] = tmp;
        }
}

TVec2 Planner::GetWayPoint ( unsigned int i )
{
        if ( i < waypoints )
                return waypoint[i];
        return Vec2 ( );
}

unsigned int Planner::GetWayPointNum ( void )
{
        return waypoints;
}

bool Planner::IsReached ( void )
{
        if ( DIS ( final_state , tree.NearestNeighbour ( final_state )->state ) <= acceptable_dis )
                return true;
        return false;
}

TVec2 Planner::Plan ( void )
{
    //return final_state;
        if ( !collisionDetect ( init_state , final_state ) )
        {
                tree.AddNode ( final_state , tree.NearestNeighbour ( init_state ) );
        }
        else
        {
                Node * new_s;
                int type = 0;
                TVec2 r;
                for ( int i = 0 ; ( ( i < MAX_NODES ) && ( !IsReached ( ) ) ) ; i ++ )
                {
                        r = choose_target ( &type );
                        new_s = extend ( tree.NearestNeighbour ( r ) , r );
                        if ( new_s )
                        {
                                /*if ( !collisionDetect ( new_s->state , final_state ) )
                                {
									for (; ( !IsReached ( ) ) && ( new_s ) && (i < MAX_NODES ) ; i ++ )
                                                new_s = extend ( new_s , final_state );
                                        //tree.AddNode ( final_state , tree.NearestNeighbour ( r ) );
                                }*/
                        }
                        //while ( !extend ( tree.NearestNeighbour ( r ) , r ) )
                        //	r = choose_target ( );
                }
                if ( ( IsReached ( ) ) && ( !IsInObstacle ( final_state ) ) && ( DIS ( final_state , tree.NearestNeighbour ( final_state ) -> state ) > 1 ) )
                {
                        tree.AddNode ( final_state , tree.NearestNeighbour ( final_state ) );
                }
        }

        SetWayPoints ( );

        //if ( ( ! IsInObstacle ( init_state ) ) && ( GetWayPointNum() > 2 ) )
			optimize_tree ( );

        TVec2 ans;
		if ( waypoints > 1 )
		{
            if ( started_in_obs )
            {
                ans.X = waypoint[waypoints-1].X;
                ans.Y = waypoint[waypoints-1].Y;
            }
            else
            {
                ans.X = waypoint[waypoints-2].X;
                ans.Y = waypoint[waypoints-2].Y;
            }
			
		}
		else
		{
			ans.X = waypoint[0].X;
			ans.Y = waypoint[0].Y;
		}

	return ans;
}

void Planner::optimize_tree ( void )
{
        for ( int i = 0 ; i < GetWayPointNum ( ) ; i ++ )
        {
                if ( collisionDetect ( waypoint[i] , waypoint[GetWayPointNum()-1] ) == false )
                {
                        TVec2 tmp = waypoint[i+1];
                        waypoint[i+1] = waypoint[GetWayPointNum()-1];
                        waypoint[GetWayPointNum()-1] = tmp;
                        waypoints = i + 2;
                        break;
                }
        }
}

