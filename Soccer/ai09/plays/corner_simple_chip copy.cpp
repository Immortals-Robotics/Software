#include "ai09.h"

void ai09::corner_simple_chip ( void )
{
	GK ( gk , 2 );
	TwoDef ( def1 , def2 );
	
	TVec2 jagir;
	if ( reached )
		jagir = Vec2(-side*2200, -sgn(ball.Position.Y )* 700 );
	else
		jagir = Vec2(-side*1400, -sgn(ball.Position.Y )* 900 );
	
	swap(attack, passgir);
	
	if ( ( ( DIS ( ball.Position , OwnRobot[attack].State.Position ) < 250 ) && ( OwnRobot[attack].State.velocity.magnitude < 100 ) )|| ( reached ) )
	{
		float waitTime = 1.0;
		if ( ball.Position.X * side > -2000 )
			waitTime = 0.5;
		timer.end();
		if ( timer.time() < waitTime )
		{
			ERRTSetObstacles ( attack );
			tech_circle(attack,90-side*90 ,0,0);
			
		}
		else if ( timer.time() < waitTime*2 )
		{
			ERRTSetObstacles ( attack );
			if ( ball.Position.X * side > -2000 )
				tech_circle(attack,AngleWith ( Vec2 ( -side*2500 , -sgn ( ball.Position.Y ) * 1500 ) , ball.Position ) ,0,0,0,1);
			else
				tech_circle(attack,AngleWith ( Vec2 ( -2730,0 ) , ball.Position ) ,0,0,0,1);
		}
		else if ( timer.time() < waitTime*3 )
		{
			ERRTSetObstacles ( attack );
			if ( ball.Position.X * side > -2000 )
				tech_circle(attack,AngleWith ( Vec2 ( -side*2500 , -sgn ( ball.Position.Y ) * 1500 ) , ball.Position ) ,0,0,0,1,1);
			else
				tech_circle(attack,AngleWith ( Vec2 ( -2730,0 ) , ball.Position ) ,0,0,0,1,1);
		}
		else
		{
			ERRTSetObstacles ( attack );
			if ( ball.Position.X * side > -2000 )
				tech_circle(attack,AngleWith ( Vec2 ( -side*2500 , -sgn ( ball.Position.Y ) * 1500 ) , ball.Position ) ,0,12,0,1);
			else
				tech_circle(attack,AngleWith ( Vec2 ( -2730,0 ) , ball.Position ) ,0,12,0,1);
		}
		reached = true;
	}
	else
	{
		ERRTSetObstacles ( attack );
		tech_circle(attack,90-side*90 ,0,0);
	}
	
	////////
	
	if ( ( fabs ( NormalizeAngle ( ball.velocity.direction - AngleWith ( ball.Position , Vec2 ( OwnRobot[passgir].State.Position.X + BAR * cosDeg ( OwnRobot[passgir].State.Angle ) , OwnRobot[passgir].State.Position.Y + BAR * sinDeg ( OwnRobot[passgir].State.Angle ) ) ) ) ) < 65 ) && ( ball.velocity.magnitude > 100 ) )//&&(abs(ball.vel_angle-90)>0.01)&&(abs(ball.vel_angle+90)>0.01)&&(abs(ball.vel_angle-180)>0.01)&&(abs(ball.vel_angle+180)>0.01))
	{
		WaitForPass ( passgir );
		hys = 30;
	}
	else if (( hys > 0 )&& ( ball.velocity.magnitude > 50 ))// &&(abs(ball.vel_angle-90)>0.01)&&(abs(ball.vel_angle+90)>0.01)&&(abs(ball.vel_angle-180)>0.01)&&(abs(ball.vel_angle+180)>0.01))
	{
		WaitForPass ( passgir );
		hys --;
	}
	else
	{
		hys = 0;
		OwnRobot[passgir].face ( Vec2 ( -side*2995 , 0 ) );
		ERRTSetObstacles ( passgir );
		timer.end();
		if ( timer.time() < 1.5 )
		{if ( ball.Position.X * side > -2000 )
			ERRTNavigate2Point ( passgir , Vec2 ( -side*2500 , -sgn(ball.Position.Y )* 1500 ) );
		else
			ERRTNavigate2Point ( passgir , jagir );										
		}
		else {
			if ( ball.Position.X * side > -2000 )
				ERRTNavigate2Point ( passgir , Vec2 ( -side*2500 , -sgn(ball.Position.Y )* 1500 ) );
			else
				ERRTNavigate2Point ( passgir , jagir );					
		}
		
	}
	
	swap(attack, passgir);
	
	//OwnRobot[def2].face ( Vec2 ( -side*2995 , 0 ) );
	//Navigate2Point ( def2 , Vec2 ( -side*1500 , 0 ) );
}