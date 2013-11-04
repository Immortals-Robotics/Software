#include "ai09.h"

void ai09::kickoff_us_chip ( bool canKickBall )
{
	GK ( gk , 2 );
	TwoDef ( def1 , def2 );
	
	OwnRobot[passgir].face ( Vec2 ( -side*2995 , 0 ) );
	Navigate2Point ( passgir , Vec2 ( side*150 , 1700 ) );
	if ( canKickBall )
	{
		tech_circle(attack,AngleWith ( Vec2 ( -side*2995,0 ) , ball.Position ), 0,1,0,1);
	}
	else
	{
		tech_circle(attack,AngleWith ( Vec2 ( -side*2995,0 ) , ball.Position ));
	}
}