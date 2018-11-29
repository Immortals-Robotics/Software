#include "Geom.h"
#include "helpers.h"

class Obstacle
{
	public:
		Circle circle;

}obstacle[100],tempobs[100],showobs[100];
int obs_num,waypoints;
VecPosition path[100];

VecPosition Plan ( VecPosition start , VecPosition end )
{
	VecPosition current = start;
	VecPosition* p1 = new VecPosition ( );
	VecPosition* p2 = new VecPosition ( );
	Rect* r;
	int colls = 0;

	waypoints = 0;
	
	while ( ( current.getDistanceTo ( end ) > 1 ) && ( waypoints < 1 ) )
	{
		r = new Rect ( VecPosition ( min ( current.getX ( ) , end.getX ( ) ) - 19 , min ( current.getY ( ) , end.getY ( ) ) - 19 ) , VecPosition ( max ( current.getX ( ) , end.getX ( ) + 19 ) , max ( current.getY ( ) , end.getY ( ) ) + 19 )  );
		int collision_index=-1;
		float pp = 1000000;

		for ( int i = 0 ; i < obs_num ; i++ )
		{
			if ( ( obstacle[i].circle.getRadius ( ) > 0 ) &&
				( r->isInside ( obstacle[i].circle.getCenter ( ) ) ) &&
				( Line::makeLineFromTwoPoints ( current , end ).getCircleIntersectionPoints ( obstacle[i].circle , p1 , p2 ) )
				)
			{
				if ( current.getDistanceTo ( obstacle[i].circle.getCenter ( ) ) < pp )
				{
					collision_index = i;
					pp = current.getDistanceTo ( obstacle[i].circle.getCenter ( ) );
				}
			}
		}

		if ( collision_index == -1 )
		{
			current = end;
			path[waypoints] = end;
			waypoints++;
		}

		else
		{
			while (!( colls = Circle( current , sqrt ( pow ( current.getDistanceTo ( obstacle[collision_index].circle.getCenter ( ) ) , 2 ) - pow ( obstacle[collision_index].circle.getRadius ( ) , 2 ) ) ).getIntersectionPoints ( obstacle[collision_index].circle , p1 , p2 ) ))
				obstacle[collision_index].circle.setRadius ( obstacle[collision_index].circle.getRadius ( ) -1 );

			if ( colls == 1 )
				path[waypoints] = *p1;
			else
			{
				if ( Line::makeLineFromTwoPoints ( current , end ).getDistanceWithPoint ( *p1 ) < Line::makeLineFromTwoPoints ( current , end ).getDistanceWithPoint ( *p2 ) )
					path[waypoints] = *p1;
				else
					path[waypoints] = *p2;
			}
			current = path[waypoints];
			//obstacle[collision_index].circle.setRadius ( 0 );
			waypoints ++;
		}
	}
	return path[0];
}
