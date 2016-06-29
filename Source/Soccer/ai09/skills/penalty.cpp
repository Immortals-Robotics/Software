#include "ai09.h"

#include "helpers.h"

void ai09::PenaltyUs ( int robot_num , float angle , int kick , int chip )
{
	TVec2 PredictedBall = ball.Position;

	float r = 180.0f;

	float tetta = 25.0f;

	if ( ( kick > 0 ) || ( chip > 0 ) )
	{
		r = 110.0f;
		tetta = 15.0f;
	}

	OwnRobot[robot_num].face ( PredictedBall );
	
	float hehe = AngleWith ( PredictedBall , OwnRobot[robot_num].State.Position );
	hehe = NormalizeAngle ( angle - hehe );

	ERRTSetObstacles ( robot_num, false, false );

	if ( fabs ( hehe ) < tetta )
	{
		//hehe = angle;
		//if ( OwnRobot[2].State.Angle < 0 )
		if (( kick) ||(chip ))
			ERRTNavigate2Point ( robot_num , CircleAroundPoint ( PredictedBall , angle , min ( r , fabs(hehe)*280.0f/tetta ) ) );
		else
		{
			//OwnRobot[robot_num].target.Angle = 90+side*90;
			ERRTNavigate2Point ( robot_num , CircleAroundPoint ( PredictedBall , angle+hehe*0.0f , r/1.5f ) ,0, 100 , &VELOCITY_PROFILE_AROOM );
		}
	}
	else
	{
		hehe = AngleWith ( PredictedBall , OwnRobot[robot_num].State.Position ) + sgn ( hehe ) * tetta;
		ERRTNavigate2Point ( robot_num , CircleAroundPoint ( PredictedBall , hehe , r ) );
	}

	if ( ( kick > 0 ) || ( chip > 0 ) )
	{
		TVec2 tmpPos = OwnRobot[robot_num].State.Position;
		float tmpAng = OwnRobot[robot_num].State.Angle;
		/*if ( side == -1 )
		{
			tmpPos.X = -tmpPos.X;
			tmpAng += 180.0f;
		}*/
		float t1 = AngleWith ( tmpPos , Vec2 ( -field_width , -450.0f ) );
		float t2 = AngleWith ( tmpPos , Vec2 ( -field_width , 450.0f ) );

		while ( t1 < 0 )
			t1 += 360.0f;
		while ( t1 > 360 )
			t1 -= 360.0f;
		while ( t2 < 0 )
			t2 += 360.0f;
		while ( t2 > 360 )
			t2 -= 360.0f;

		float ot = tmpAng;
		while ( ot < 0 )
			ot += 360.0f;
		while ( ot > 360 )
			ot -= 360.0f;

		//if ( ( ot > t2 ) && ( ot < t1 ) )
		//if ( fabs ( NormalizeAngle ( OwnRobot[robot_num].State.Angle - AngleWith ( OwnRobot[robot_num].State.Position , Vec2 ( -3025 , 0 ) ) ) ) < 20 ) 
		if ( ( ( side == -1 ) && ( fabs ( OwnRobot[robot_num].State.Angle ) < 120 ) ) ||
			 ( ( side == 1 ) && ( fabs ( OwnRobot[robot_num].State.Angle ) > 60 ) ) )
		{
			if ( fabs ( NormalizeAngle ( 180+angle - OwnRobot[robot_num].State.Angle ) ) < 30 )
			{
				//if ( chip )
				OwnRobot[robot_num].Chip(chip);
				//else
				OwnRobot[robot_num].Shoot(kick);
			}
		}
	}
}