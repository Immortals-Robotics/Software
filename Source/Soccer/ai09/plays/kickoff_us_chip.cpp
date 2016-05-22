#include "../ai09.h"

void ai09::kickoff_us_chip ( void )
{
	bool canKickBall = bool(currentPlayParam);
	GKHi ( gk );
	DefHi ( def );
	
	ERRTSetObstacles ( dmf , true , true , true , true );
	OwnRobot[dmf].face(ball.Position);
	ERRTNavigate2Point ( dmf , PointOnConnectingLine(ball.Position, Vec2(side*field_width, 0), DIS(ball.Position, Vec2(side*field_width, 0))/3.0f) ,0 , 40,&VELOCITY_PROFILE_MAMOOLI);
	
	if (timer.time()<0.5) {
		if ( OwnRobot[mid1].State.Position.Y > OwnRobot[mid2].State.Position.Y )
		{ 
			swap(mid1, mid2);
		}
	}
	
	OwnRobot[mid2].face ( Vec2 ( -side*field_width , 0 ) );
	ERRTSetObstacles ( mid2 , true , true , true , true );
	ERRTNavigate2Point ( mid2 , Vec2 ( ball.Position.X+side*150 , (field_height-300) ) );
	OwnRobot[mid1].face ( Vec2 ( -side*field_width , 0 ) );
	ERRTSetObstacles ( mid1 , true , true , true , true );
	ERRTNavigate2Point ( mid1 , Vec2 ( ball.Position.X+side*150 , -(field_height-300) ) );
	TVec2 chip_target = Vec2(-side*2000, 0);
	if ( canKickBall )
	{
		float chip_pow = 85;//DIS(chip_target, ball.Position)/11;
		tech_circle(attack,AngleWith ( chip_target , ball.Position ), 0,chip_pow,0,1,0,1);
		//circle_ball(attack, AngleWith ( chip_target , ball.Position ), 100, 0, 1.0f);
	}
	else
	{
		//tech_circle(attack,AngleWith ( chip_target , ball.Position ),0,0,0,1,0,1);
		circle_ball(attack, AngleWith ( chip_target , ball.Position ), 0, 0, 1.0f);
	}
}