#include "obstacle_new.h"
#include <limits.h>

ObsMap::ObsMap ( unsigned int _maxObs )
{
	obstacle = new BaseObstacle * [_maxObs];

	obsNum = 0;
	maxObs = _maxObs;
}

void ObsMap::AddCircle ( float _x , float _y , float _r )
{
	if ( ( _r > 0 ) && ( obsNum < maxObs ) )
	{
		obstacle[obsNum++] = new CircleObstacle ( _x , _y , _r );
	}
}
void ObsMap::AddRectangle ( float _x , float _y , float _w , float _h )
{
	if ( ( _w > 0 ) && ( _h > 0 ) && ( obsNum < maxObs ) )
	{
		obstacle[obsNum++] = new RectangleObstacle ( _x , _y , _w , _h );
	}
}

void ObsMap::AddObstacle ( BaseObstacle * obs )
{
	if ( ( obs ) && ( obsNum < maxObs ) )
	{
		obstacle[obsNum++] = obs;
	}
}

bool ObsMap::IsInObstacle ( float _x , float _y )
{
	for ( int i = 0 ; i < obsNum ; i ++ )
	{
		if ( obstacle[i] -> IsInObstacle ( _x , _y ) )
			return true;
	}

	return false;
}

float ObsMap::NearestDistance ( float _x , float _y )
{
    if ( obsNum == 0 )
        return INT_MAX;
    float dis = obstacle[0] -> NearestDistance( _x , _y );
    float tmp_dis;
    for ( int i = 0 ; i < obsNum ; i ++ )
    {
        tmp_dis = obstacle[i] -> NearestDistance ( _x , _y );
        if ( tmp_dis < dis )
            dis = tmp_dis;
        if ( dis <= 0 )
            return dis;
    }

    return dis;
}

void ObsMap::resetMap ( void )
{
	for ( int i = 0 ; i < obsNum ; i ++ )
    {
		delete obstacle[i];
	}

	free ( obstacle );

	obstacle = new BaseObstacle * [maxObs];

	obsNum = 0;
}

int ObsMap::getObsNum ( void )
{
	return obsNum;
}
