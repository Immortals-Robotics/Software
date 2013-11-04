#include "ai09.h"

void ai09::kickoff_us_pass ( bool canKickBall )
{
	GK ( gk , 2 );
	TwoDef ( def1 , def2 );
	
	int oneTouchSide = 1;
	if ( canKickBall )
	{
		if (( ( DIS ( ball.Position , OwnRobot[attack].State.Position ) < 250 ) && ( OwnRobot[attack].State.velocity.magnitude < 100 ) )|| ( reached ) )
		{
			ERRTSetObstacles ( attack );
			tech_circle(attack,AngleWith ( Vec2 ( side*80 , oneTouchSide * 1700 ) , ball.Position ) ,2,0);
			reached = true;
		}
		else
		{
			ERRTSetObstacles ( attack );
			tech_circle(attack,AngleWith ( Vec2 ( side*80 , oneTouchSide * 1700 ) , ball.Position ) ,0,0);
		}
	}
	else
	{
		ERRTSetObstacles ( attack );
		tech_circle(attack,90-sgn(side)*90 ,0);
	}
	
	
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
		ERRTSetObstacles ( passgir , true );
		ERRTNavigate2Point ( passgir , Vec2 ( side*150 , oneTouchSide * 1700 ) );
	}
}