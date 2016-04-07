#include "../ai09.h"

void ai09::kickoff_us_pass ( void )
{
	bool canKickBall = bool(currentPlayParam);
	GKHi(gk, 1, 0);
	DefHi(def);
	
	ERRTSetObstacles ( dmf , true , true , true , true );
	OwnRobot[dmf].face(ball.Position);
	ERRTNavigate2Point ( dmf , PointOnConnectingLine(ball.Position, Vec2(side*field_width, 0), DIS(ball.Position, Vec2(side*field_width, 0))/3.0f) ,0 , 40,&VELOCITY_PROFILE_MAMOOLI);
	
	
    
	int oneTouchSide = 1;
	if ( canKickBall )
	{
		/*if (( ( DIS ( ball.Position , OwnRobot[attack].State.Position ) < 155 ) && ( OwnRobot[attack].State.velocity.magnitude < 100 ) )|| ( reached ) )
		{
			ERRTSetObstacles ( attack );
			tech_circle(attack,AngleWith ( Vec2 ( -side*80 , oneTouchSide * 1700 ) , ball.Position ) ,4,0,0,1);
			reached = true;
		}
		else
		{
			ERRTSetObstacles ( attack );
			tech_circle(attack,AngleWith ( Vec2 ( -side*80 , oneTouchSide * 1700 ) , ball.Position ) ,0,0,0,1);
		}*/

		TVec2 pass_pos;
		int shoot_pow = 0;
		int chip_pow = 0;
		if (OwnRobot[mid1].State.seenState != CompletelyOut && OwnRobot[mid2].State.seenState != CompletelyOut)
		{
			pass_pos = OwnRobot[(randomParam<0.0)?mid1:mid2].State.Position;
			pass_pos.X += -side*900.0f;
			shoot_pow = 30;
		}
		else if (OwnRobot[mid1].State.seenState != CompletelyOut)
		{
			pass_pos = OwnRobot[mid1].State.Position;
			pass_pos.X += -side*900.0f;
			shoot_pow = 30;
		}
		else if (OwnRobot[mid2].State.seenState != CompletelyOut)
		{
			pass_pos = OwnRobot[mid2].State.Position;
			pass_pos.X += -side*900.0f;
			shoot_pow = 30;
		}
		else
		{
			pass_pos = Vec2(-side * field_width, 0);
			chip_pow = 200;
		}
		circle_ball(attack, AngleWith ( pass_pos , ball.Position ), shoot_pow, chip_pow, 1.0f);
		
	}
	else
	{
		ERRTSetObstacles ( attack );
		tech_circle(attack,90-sgn(side)*90 ,0);
	}

	allafPos[mid1] = Vec2 ( side*150 , -oneTouchSide * 2700 );
	allafPos[mid2] = Vec2 ( side*150 , oneTouchSide * 2700 );

	recievePass(mid1, allafPos[mid1]);
	recievePass(mid2, allafPos[mid2]);

	oneTouchType[mid1] = oneTouch;
    oneTouchType[mid2] = oneTouch;
}