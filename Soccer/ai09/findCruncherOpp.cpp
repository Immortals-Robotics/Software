#include "ai09.h"

int ai09::findCruncherOpp ( int mask1 , int mask2 )
{
	float mdis = 7000;
	int index = -1;
	for ( int i = 0 ; i < 12 ; i ++ )
	{
		if ( ( i == mask1 ) || ( i == mask2 ) )
			continue;
		if ( OppRobot[i].seenState == CompletelyOut )
			continue;
		if ( ( fabs ( OppRobot[i].Position.X ) > 3025 ) ||
			( fabs ( OppRobot[i].Position.Y ) > 2025 ) )
			continue;
		if ( ( DIS ( Vec2 ( side*3025 , 0 ) , OppRobot[i].Position ) < mdis ) && (( DIS ( ball.Position , OppRobot[i].Position ) > 500 )) )
		{
			mdis = DIS ( Vec2 ( side*3025 , 0 ) , OppRobot[i].Position );
			index = i;
		}
	}
	return index;
}