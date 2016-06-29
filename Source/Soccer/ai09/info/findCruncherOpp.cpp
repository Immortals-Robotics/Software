#include "ai09.h"

int ai09::findCruncherOpp ( int mask1 , int mask2 , bool acceptNearBall )
{
	float mdis = 7000;
	int index = -1;
	for ( int i = 0 ; i < 12 ; i ++ )
	{
		if ( ( i == mask1 ) || ( i == mask2 ) )
			continue;
		if ( OppRobot[i].seenState == CompletelyOut )
			continue;
		if ( ( fabs ( OppRobot[i].Position.X ) > field_width ) ||
			( fabs ( OppRobot[i].Position.Y ) > field_height ) )
			continue;
		if ((!acceptNearBall)&&(( DIS ( ball.Position , OppRobot[i].Position ) < 500 ))) {
			continue;
		}
		if ( ( DIS ( Vec2 ( side*field_width , 0 ) , OppRobot[i].Position ) < mdis ) )
		{
			mdis = DIS ( Vec2 ( side*field_width , 0 ) , OppRobot[i].Position );
			index = i;
		}
	}
	return index;
}