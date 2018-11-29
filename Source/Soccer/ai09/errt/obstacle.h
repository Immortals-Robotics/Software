#pragma once

/*bool obs_lut[605][405];

void clear_map ( void )
{
	for ( int i = 0 ; i < 605 ; i ++ )
		for ( int j = 0 ; j < 405 ; j ++ )
		{
			obs_lut[i][j] = false;
		}
}

bool IsInObstacle ( TVec2 p )
{
	return obs_lut[(int)p.X][(int)p.Y];
}

bool collisionDetect ( TVec2 p1 , TVec2 p2 )
{
	float coss , sinn;
	coss = ( p2.X - p1.X ) / DIS ( p1 , p2 );
	sinn = ( p2.Y - p1.Y ) / DIS ( p1 , p2 );

	TVec2 current = p1;

	while ( DIS ( current , p2 ) > 9 )
	{
		if ( IsInObstacle ( current ) )
			return true;

		current.X += coss * 9;
		current.Y += sinn * 9;
	}

	return false;
}

void AddCircle ( int x , int y , int r )
{
	for ( int i = 0 ; i < 605 ; i ++ )
		for ( int j = 0 ; j < 405 ; j ++ )
		{
			//obs_lut[i][j] = false;
			if ( (i-x) * (i-x) + (j-y) * (j-y) < r * r )	obs_lut[i][j] = true;
		}
}*/

#include "obstacle_new.h"
#include "../../../Common/Vector.h"

void clear_map ( void );
bool IsInObstacle ( TVec2 p );
bool collisionDetect ( TVec2 p1 , TVec2 p2 );
void AddCircle ( float x , float y , float r );
void AddRectangle ( float x , float y , float w , float h );