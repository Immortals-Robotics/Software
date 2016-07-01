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

	TVec2 openAngle = calculateOpenAngleToGoal(ball.Position, attack);

	float target;
	float shoot_pow = 0;
	float chip_pow = 0;

	if (openAngle.Y < 6)
	{
		TVec2 chip_target = Vec2(-side * 2000, 0);
		target = AngleWith(chip_target, ball.Position);
		chip_pow = 105;
	}
	else
	{
		if (openAngle.Y > 10)
			target = NormalizeAngle(180 + openAngle.X);
		else
			target = AngleWith(Vec2(-side*field_width, 0), ball.Position);

		shoot_pow = 50;
	}


	if ( canKickBall )
	{
		circle_ball(attack, target, shoot_pow, chip_pow, 1.0f);
		
	}
	else
	{
		circle_ball(attack, target, 0, 0, 1.0f);
	}
}