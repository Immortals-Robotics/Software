#include "../ai09.h"

void ai09::throwin_tu_omgh ( void )
{
	side=-side;
	GK ( gk , 2 );
	TwoDef ( def , dmf );
	Halt(lmf);
	side=-side;
	
	if ( ( ( DIS ( ball.Position , OwnRobot[attack].State.Position ) < 250 ) && ( OwnRobot[attack].State.velocity.magnitude < 100 ) )|| ( reached ) )
	{
		float waitTime = 1.5;
		if ( ball.Position.X * side > -2000 )
			waitTime = 1.5;
		if ( timer.time() < waitTime )
		{
			ERRTSetObstacles ( attack );
			tech_circle(attack,90-side*90 ,0,0,0,1,0,0);
			
		}
		else
		{
			ERRTSetObstacles ( attack );
			tech_circle(attack,AngleWith ( Vec2 ( -side*2000 , -sgn ( ball.Position.Y ) * 1600 ) , ball.Position ) ,5,0,0,1,0,0);
		}
		reached = true;
	}
	else
	{
		ERRTSetObstacles ( attack );
		tech_circle(attack,90-side*90 ,0,0,0,1,0,0);
	}
	
	if ( oneTouchDetector[rmf].IsArriving(70) )
	{
		WaitForOmghi ( rmf );
		hys = 30;
	}
	else
	{
		hys = 0;
		OwnRobot[rmf].face ( Vec2 ( -side*2995 , 0 ) );
		ERRTSetObstacles ( rmf );
		if ( timer.time() < 0.0 )
		{
			ERRTNavigate2Point ( rmf , Vec2 ( -side*2300 , -sgn(ball.Position.Y )* 1000 ) );										
		}
		else {
			ERRTNavigate2Point ( rmf , Vec2 ( -side*200 , -sgn(ball.Position.Y )* 1600 ) );					
		}
		
	}
	
	//OwnRobot[def2].face ( Vec2 ( -side*2995 , 0 ) );
	//Navigate2Point ( def2 , Vec2 ( -side*1500 , 0 ) );
}