#include "ai09.h"

int ai09::findNearestAsshole ( TVec2 pos , int mask , bool acceptNearBall )
{
	float mdis = 7000;
	int index = -1;
	for ( int i = 0 ; i < MAX_ROBOTS ; i ++ )
	{
		if ( i == mask )
			continue;
		if ( OppRobot[i].seenState == CompletelyOut )
			continue;
		if ( ( fabs ( OppRobot[i].Position.X ) > field_width ) ||
			( fabs ( OppRobot[i].Position.Y ) > field_height ) )
			continue;
		if ((!acceptNearBall)&&(( DIS ( ball.Position , OppRobot[i].Position ) < 500 ))) {
			continue;
		}
		if ( ( DIS ( pos , OppRobot[i].Position ) < mdis ) )
		{
			mdis = DIS ( pos , OppRobot[i].Position );
			index = i;
		}
	}
	
	return index;
}