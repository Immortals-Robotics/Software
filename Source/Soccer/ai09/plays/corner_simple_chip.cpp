#include "../ai09.h"

void ai09::corner_simple_chip ( void )
{
	//GK ( gk , 1 );
	//OneDef ( def );
	
	GKHi(gk, 1, 0);
	DefHi(def, NULL, false);
	
	ERRTSetObstacles ( dmf , false , true , true , true );
	OwnRobot[dmf].face(ball.Position);
	ERRTNavigate2Point ( dmf , PointOnConnectingLine(ball.Position, Vec2(side*3025, 0), 1500) ,0 , 100,&VELOCITY_PROFILE_MAMOOLI);
	
	Halt(lmf);
	
	TVec2 jagir;
	if ( reached )
		jagir = Vec2(-side*2200, -sgn(ball.Position.Y )* 700 );
	else
		jagir = Vec2(-side*1400, -sgn(ball.Position.Y )* 900 );
	
	//swap(attack, passgir);
	
	if ( ( ( DIS ( ball.Position , OwnRobot[attack].State.Position ) < 250 ) && ( OwnRobot[attack].State.velocity.magnitude < 100 ) )|| ( reached ) )
	{
		float waitTime = 2.0;
		if ( ball.Position.X * side > -2000 )
			waitTime = 2.5;
		timer.end();
		if ( timer.time() < waitTime )
		{
			ERRTSetObstacles ( attack );
			tech_circle(attack,90-side*90 ,0,0,0,1,0,0);
			
		}
		else if ( timer.time() < waitTime*2 )
		{
			ERRTSetObstacles ( attack );
			tech_circle(attack,AngleWith ( Vec2 ( ball.Position.X,-2000 ) , ball.Position ) ,0,0,0,1,0,0);
		}
		else if ( timer.time() < waitTime*2 )
		{
			ERRTSetObstacles ( attack );
			tech_circle(attack,AngleWith ( Vec2 ( ball.Position.X,-2000 ) , ball.Position ) ,0,0,0,1,0,0);
		}
		else
		{
			ERRTSetObstacles ( attack );
			tech_circle(attack,AngleWith ( Vec2 ( ball.Position.X,-2000 ) , ball.Position ) ,0,5,0,1,0,0);
		}
		reached = true;
	}
	else
	{
		ERRTSetObstacles ( attack );
		tech_circle(attack,90-side*90 ,0,0);
	}
	
	////////
	
	if (oneTouchDetector[rmf].IsArriving(70)) {
		WaitForPass ( rmf );
	}
	else {
		OwnRobot[rmf].face ( Vec2 ( -side*2995 , 0 ) );
		ERRTSetObstacles(rmf, 1, 1, 1, 0, 0, 1);
		if ( timer.time() < 1.5 )
		{if ( ball.Position.X * side > -2000 )
			ERRTNavigate2Point ( rmf , Vec2 ( -side*2500 , -sgn(ball.Position.Y )* 1500 ) );
		else
			ERRTNavigate2Point ( rmf , jagir );										
		}
		else {
			if ( ball.Position.X * side > -2000 )
				ERRTNavigate2Point ( rmf , Vec2 ( -side*2500 , -sgn(ball.Position.Y )* 1500 ) );
			else
				ERRTNavigate2Point ( rmf , jagir );					
		}
	}
}