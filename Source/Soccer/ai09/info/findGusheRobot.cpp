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
    float minDis = (field_width+field_height)*2.0;
    int ans = -1;
	for ( int i = 0 ; i < 12 ; i ++ )
	{
		if ( i == mask )
			continue;
        if ( i == oppGK )
            continue;
		if ( OppRobot[i].seenState == CompletelyOut )
			continue;
		if ( ( fabs ( OppRobot[i].Position.X ) > field_width ) ||
			( fabs ( OppRobot[i].Position.Y ) > field_height ) )
			continue;
		
		if ( isGooshe(i,0) ){
            float newDis = DIS(OppRobot[i].Position, Vec2(side*field_width, 0));
            if ( newDis < minDis )
            {
                minDis = newDis;
                ans = i;
            }
		}
	}
	
	return ans;
}