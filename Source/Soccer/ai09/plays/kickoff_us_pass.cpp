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
		circle_ball(attack, AngleWith ( OwnRobot[(randomParam<0.5)?rmf:lmf].State.Position , ball.Position ), 30, 0, 1.0f);
		
	}
	else
	{
		ERRTSetObstacles ( attack );
		tech_circle(attack,90-sgn(side)*90 ,0);
	}
	
	if (oneTouchDetector[rmf].IsArriving(70)) {
		WaitForPass ( rmf );
	}
	else
	{
		OwnRobot[rmf].face ( Vec2 ( -side*field_width , 0 ) );
		ERRTSetObstacles ( rmf , true );
		ERRTNavigate2Point ( rmf , Vec2 ( side*150 , oneTouchSide * 2700 ) );
	}
	
	if (oneTouchDetector[lmf].IsArriving(70)) {
		WaitForPass ( lmf );
	}
	else
	{
		OwnRobot[lmf].face ( Vec2 ( -side*field_width , 0 ) );
		ERRTSetObstacles ( lmf , true );
		ERRTNavigate2Point ( lmf , Vec2 ( side*150 , -oneTouchSide * 2700 ) );
	}
	
    oneTouchType[rmf] = oneTouch;
    oneTouchType[lmf] = oneTouch;
}