#include "ai09.h"

void ai09::kickoff_us_farar ( bool canKickBall )
{
	GK(gk, 2);
	TwoDef(def1, def2);
	
	if ( canKickBall )
	{
		tech_circle(attack,AngleWith ( Vec2 ( -side*2995,0 ) , ball.Position ), 0,15,0,1);
		
		OwnRobot[passgir].face ( Vec2 ( -side*2995 , 0 ) );
		ERRTSetObstacles(passgir, 0, 1, 1, 1);
		ERRTNavigate2Point ( passgir , Vec2 ( -side*1500 , 0 ) );
	}
	else
	{
		tech_circle(attack,AngleWith ( Vec2 ( -side*2995,0 ) , ball.Position ));

		OwnRobot[passgir].face ( Vec2 ( -side*2995 , 0 ) );
		ERRTSetObstacles(passgir, 0, 1, 1, 1);
		AddCircle(ball.Position.X, ball.Position.Y, 15);
		ERRTNavigate2Point ( passgir , Vec2 ( side*100 , -250 ) );		
	}
}