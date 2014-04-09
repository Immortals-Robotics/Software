#include "ai09.h"

bool ai09::isGooshe ( int id , bool sameSideAsBall )
{
	/*TVec2 search_center = Vec2(side*2900, -sgn(ball.Position.Y)*1900);
	if (sameSideAsBall) {
		search_center.Y = -search_center.Y;
	}
	
	if ( DIS(OppRobot[id].Position, search_center) < 1700 )
		return true;
	
	return false;*/
	
	int ballYsgn = sgn(ball.Position.Y);
	if ( !sameSideAsBall )
		ballYsgn = - ballYsgn;
	
	if ( ( OppRobot[id].Position.X * side > 500 ) && ( ballYsgn * OppRobot[id].Position.Y > 1000 ) )
		return true;
	return false;
	
}

int ai09::findGusheRobot ( int mask )
{
	for ( int i = 0 ; i < 12 ; i ++ )
	{
		if ( i == mask )
			continue;
        if ( i == oppGK )
            continue;
		if ( OppRobot[i].seenState == CompletelyOut )
			continue;
		if ( ( fabs ( OppRobot[i].Position.X ) > 3025 ) ||
			( fabs ( OppRobot[i].Position.Y ) > 2025 ) )
			continue;
		
		if ( isGooshe(i,0) ){
			return i;
		}
	}
	
	return -1;
}