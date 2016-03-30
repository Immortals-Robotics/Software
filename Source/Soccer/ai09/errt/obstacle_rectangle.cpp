#include "obstacle_new.h"
#include "../helpers.h"
#include "../../../Common/distance.h"

RectangleObstacle::RectangleObstacle ( float _x , float _y , float _w , float _h )
{
	x = _x;
	y = _y;
	w = _w;
	h = _h;
}

bool RectangleObstacle::IsInObstacle ( float _x , float _y )
{
	if ( ( _x < x + w ) &&
		 ( _x > x     ) &&
		 ( _y < y + h ) &&
		 ( _y > y     ) )
		return true;

	return false;
}

float RectangleObstacle::NearestDistance ( float _x , float _y )
{
    if ( IsInObstacle( _x , _y ) )
        return -1.0f;

    if ( ( _x > x ) && ( _x < x + w ) )
    {
        return min ( fabs ( _y - y ) , fabs ( _y - y - h ) );
    }

    if ( ( _y > y ) && ( _y < y + h ) )
    {
        return min ( fabs ( _x - x ) , fabs ( _x - x - w ) );
    }

    if ( ( _x < x ) && ( _y < y ) )
    {
        return DIS( _x , _y , x , y );
    }

    if ( ( _x < x ) && ( _y > y + h ) )
    {
        return DIS( _x , _y , x , y+h );
    }

    if ( ( _x > x + w ) && ( _y < y ) )
    {
        return DIS( _x , _y , x+w , y );
    }

    if ( ( _x > x + w ) && ( _y > y + h ) )
    {
        return DIS( _x , _y , x+w , y+h );
    }


    return 0;
}
