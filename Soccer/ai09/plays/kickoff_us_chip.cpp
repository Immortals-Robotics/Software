#include "ai09.h"

void ai09::kickoff_us_chip ( bool canKickBall )
{
	GK_Ghuz ( gk );
	DefGhuz ( def );
	
	ERRTSetObstacles ( dmf , true , true , true , true );
	OwnRobot[dmf].face(ball.Position);
	ERRTNavigate2Point ( dmf , PointOnConnectingLine(ball.Position, Vec2(side*3025, 0), DIS(ball.Position, Vec2(side*3025, 0))/3.0f) ,0 , 40,&VELOCITY_PROFILE_MAMOOLI);
		
	OwnRobot[rmf].face ( Vec2 ( -side*2995 , 0 ) );
	Navigate2Point ( rmf , Vec2 ( ball.Position.X+side*150 , 1700 ) );
	OwnRobot[lmf].face ( Vec2 ( -side*2995 , 0 ) );
	Navigate2Point ( lmf , Vec2 ( ball.Position.X+side*150 , -1700 ) );
	if ( canKickBall )
	{
		tech_circle(attack,AngleWith ( Vec2 ( -side*2995,0 ) , ball.Position ), 0,7,0,1,0,1);
	}
	else
	{
		tech_circle(attack,AngleWith ( Vec2 ( -side*2995,0 ) , ball.Position ),0,0,0,1,0,1);
	}
}