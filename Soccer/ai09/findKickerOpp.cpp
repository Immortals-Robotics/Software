#include "ai09.h"

int ai09::findKickerOpp ( int mask )
{
	float mdis = 500;
	int index = -1;
	for ( int i = 0 ; i < 12 ; i ++ )
	{
		if ( i == mask )
			continue;
		if ( OppRobot[i].seenState == CompletelyOut )
			continue;
		if ( DIS ( ball.Position , OppRobot[i].Position ) < mdis )
		{
			mdis = DIS ( ball.Position , OppRobot[i].Position );
			index = i;
		}
	}
	return index;
}