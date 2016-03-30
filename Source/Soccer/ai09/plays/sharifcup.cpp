#include "../ai09.h"

int sharifDir = -1;
int obs_i = 0;

void ai09::sharifcup_pre_start ( void )
{
	ERRTSetObstacles(cmf, 0, 0, 0, 0, 0, 0);
	OwnRobot[cmf].target.Angle = 180;
	//ERRTNavigate2Point(cmf, Vec2(3360, 1630), 0, 100, &VELOCITY_PROFILE_AROOM);
	Navigate2Point(cmf, Vec2(900, 0), 0, 100, &VELOCITY_PROFILE_AROOM);
}

void ai09::sharifcup_play ( void )
{
	ERRTSetObstacles(cmf, 0, 0, 0, 0, 0, 0);
	OwnRobot[cmf].target.Angle = 90;
	ERRTNavigate2Point(cmf, Vec2(200, -1500), 0, 100, &VELOCITY_PROFILE_AROOM);
}

void ai09::sharifcup_play_2nd ( void )
{
	ERRTSetObstacles(cmf, 0, 0, 0, 0, 0, 0);
	AddRectangle(700, -1050, 210, 210);
	OwnRobot[cmf].target.Angle = 90;
	ERRTNavigate2Point(cmf, Vec2(200, -1500), 0, 100, &VELOCITY_PROFILE_AROOM);
}

void ai09::sharifcup_play_3rd ( void )
{
	float passAngle = AngleWith ( Vec2 ( 3300 , 0 ) , ball.Position );
	tech_circle(cmf, passAngle, 100, 120, 0, 0, 0, 0);
}

void ai09::sharifcup_play_4th ( void )
{
	if ( ball.seenState == CompletelyOut )
	{
		OwnRobot[cmf].target.Angle = 180;
		Navigate2Point(cmf, Vec2(900, 0), 0, 100, &VELOCITY_PROFILE_AROOM);
	}
	else
	{
		float passAngle = AngleWith ( Vec2 ( -3300 , 0 ) , ball.Position );
		//tech_circle(cmf, passAngle, 100, 120, 0, 0, 0, 0);
		tech_circle(cmf, passAngle, 100, 100, 0, 0, 0, 0);
		//Navigate2Point(cmf, ball.Position, 0, 100, &VELOCITY_PROFILE_AROOM);
	}
}

void ai09::sharifcup_post_play ( void )
{
	ERRTSetObstacles(cmf, 0, 0, 0, 0, 0, 0);
	OwnRobot[cmf].target.Angle = 90;
	ERRTNavigate2Point(cmf, Vec2(100, -1600), 0, 100, &VELOCITY_PROFILE_AROOM);
}