#include "obstacle_new.h"
#include "../../../Common/distance.h"

CircleObstacle::CircleObstacle ( float _x , float _y , float _r )
{
	x = _x;
	y = _y;
	r = _r;
}

bool CircleObstacle::IsInObstacle ( float _x , float _y )
{
	if ( ( _x > x + r ) ||
		 ( _x < x - r ) ||
		 ( _y > y + r ) ||
		 ( _y < y - r ) )
		return false;

        if ( DIS ( x , y , _x , _y ) <= r )
		return true;

        return false;
}

float CircleObstacle::NearestDistance ( float _x , float _y )
{
    return DIS ( x , y , _x , _y ) - r;
}
