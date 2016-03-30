#include "../ai09.h"

void ai09::corner_simple_pass ( void )
{
	bool cmu = bool(currentPlayParam);
	GK ( gk , 1 );
	OneDef ( def );
	//Halt(dmf);
	ERRTSetObstacles(dmf, 1, 1, 1, 1, 0);
	ERRTNavigate2Point(dmf, Vec2(side*1200, 0), 0, 100, &VELOCITY_PROFILE_AROOM);
	Halt(lmf);
	if ( ball.Position.X * side > 2000 )
	{
		swap(attack, rmf);
	}
	
	if (cmu) {
		backPass(attack,AngleWith ( Vec2 ( ball.Position.X +side*700 , -sgn ( ball.Position.Y ) * 1600 ) , ball.Position ) ,0);
	}
	else {
		if ( ( ( DIS ( ball.Position , OwnRobot[attack].State.Position ) < 250 ) && ( OwnRobot[attack].State.velocity.magnitude < 100 ) )|| ( reached ) )
		{
			float waitTime = 0.5;
			if ( ball.Position.X * side > -2000 )
				waitTime = 2.5;
			timer.end();
			if ( timer.time() < waitTime )
			{
				ERRTSetObstacles ( attack );
				tech_circle(attack,90-side*90 ,0,0);
				
			}
			else
			{
				ERRTSetObstacles ( attack );
				 if ( ball.Position.X * side > 2000 )
				 {
				 tech_circle(attack,AngleWith ( Vec2 ( -side*3500 , -sgn ( ball.Position.Y ) * 1600 ) , ball.Position ) ,0,15,0,1);
				 }
				 else if ( ball.Position.X * side > -2000 )
				 {
				 tech_circle(attack,AngleWith ( Vec2 ( ball.Position.X +side*700 , -sgn ( ball.Position.Y ) * 1600 ) , ball.Position ) ,7,0,0,1);
				 }
				 else
				 tech_circle(attack,AngleWith ( Vec2 ( -side*1600 , -sgn ( ball.Position.Y ) * 1000 ) , ball.Position ) ,7,0,0,1);
				//backPass(attack, AngleWith ( Vec2 ( -side*1600 , -sgn ( ball.Position.Y ) * 1000 ) , ball.Position ), 0);
				}
				 /*reached = true;
				 }
				 else
				 {
				 ERRTSetObstacles ( attack );
				 tech_circle(attack,90-side*90 ,0,0);
				 }*/
	}
	}

	
	
	
	
	if ( oneTouchDetector[rmf].IsArriving(70) )
	{
		WaitForPass ( rmf );
		hys = 30;
	}
	else if (( hys > 0 )&& ( ball.velocity.magnitude > 50 ))// &&(abs(ball.vel_angle-90)>0.01)&&(abs(ball.vel_angle+90)>0.01)&&(abs(ball.vel_angle-180)>0.01)&&(abs(ball.vel_angle+180)>0.01))
	{
		WaitForPass ( rmf );
		hys --;
	}
	else
	{
		hys = 0;
		OwnRobot[rmf].face ( Vec2 ( -side*2995 , 0 ) );
		ERRTSetObstacles ( rmf );
		if ( timer.time() < 1.5 )
		{
		if ( ball.Position.X * side > 2000 )
		{
			ERRTNavigate2Point ( rmf , Vec2 ( -side*2900 , -sgn(ball.Position.Y )* 1800 ) );
		}
		else if ( ball.Position.X * side > -2000 )
		{
			ERRTNavigate2Point ( rmf , Vec2 ( ball.Position.X -side*300 , -sgn(ball.Position.Y )* 1500 ) );
		}
		else
			ERRTNavigate2Point ( rmf , Vec2 ( -side*1600 , -sgn(ball.Position.Y )* 1000 ) );										
		}
		else {
			if ( ball.Position.X * side > 2000 )
			{
				ERRTNavigate2Point ( rmf , Vec2 ( -side*2500 , -sgn(ball.Position.Y )* 1500 ) );
			}
			else if ( ball.Position.X * side > -2000 )
			{
				ERRTNavigate2Point ( rmf , Vec2 ( ball.Position.X +side*700 , -sgn(ball.Position.Y )* 1500 ) );
			}
			else
				ERRTNavigate2Point ( rmf , Vec2 ( -side*1600 , -sgn(ball.Position.Y )* 1000 ) );					
		}
		
	}
	
	//OwnRobot[def2].face ( Vec2 ( -side*2995 , 0 ) );
	//Navigate2Point ( def2 , Vec2 ( -side*1500 , 0 ) );
	
	if ( ball.Position.X * side > 2000 )
	{
		swap(attack, rmf);
	}
}