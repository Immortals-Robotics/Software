#include "ai09.h"

void ai09::corner_switch_pass ( void )
{
	TVec2 goolZanak1;
	TVec2 goolZanak2;
	
	if ( ball.Position.X * side > -2000 )
	{
		goolZanak1 = Vec2 ( -side*2500 , -sgn ( ball.Position.Y ) * 1500 );
		goolZanak2 = Vec2 ( ball.Position.X +side*700, -sgn ( ball.Position.Y ) * 1500 );
	}
	else
	{
		goolZanak1 = Vec2 ( -side*1600 , -sgn ( ball.Position.Y ) * 1000 );
		goolZanak2 = Vec2 ( -side*600 , -sgn ( ball.Position.Y ) * 0 );
	}
		
	if (reached)//( ( DIS ( ball.Position , OwnRobot[attack].State.Position ) < 250 ) && ( OwnRobot[attack].State.velocity.magnitude < 100 ) )|| ( reached ) )
	{
		GK(gk, 1);
		/*OneDef(def1);
		
		OwnRobot[def2].face(Vec2(-side*3025, 0));
		ERRTSetObstacles(def2, 1, 1, 1, 1,0);
		ERRTNavigate2Point(def2, Vec2(side*800, sgn(ball.Position.Y)*1400), 0, 100);*/
		TwoDef(def1, def2);
		
		timer.end();
		if ( timer.time() < 3.5 )
		{
			ERRTSetObstacles ( attack );
			tech_circle(attack,AngleWith ( goolZanak1 , ball.Position ) ,0,0);
		}
		else
		{
			ERRTSetObstacles ( attack );
			tech_circle(attack,AngleWith ( goolZanak2 , ball.Position ) ,2,0,0,1);
		}
		
		//reached = true;
	}
	else
	{
		GK ( gk , 2 );
		TwoDef ( def1 , def2 );
		
		ERRTSetObstacles ( attack );
		tech_circle(attack,90-side*90 ,0,0);

		timer.end();
		if ( timer.time() > 1.5 )
		{
			//if ( !reached )
			//swap(attack, passgir);
			int hehet = OwnRobot[passgir].vision_id;
			OwnRobot[passgir].set_vision_id(OwnRobot[def2].vision_id);
			OwnRobot[def2].set_vision_id(hehet);
			reached = true;
		}
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
		ERRTSetObstacles ( passgir );
		timer.end();
		if ( timer.time() < 1.5 )
		{
			ERRTNavigate2Point ( passgir , goolZanak1 );
		}
		else 
		{
			ERRTNavigate2Point ( passgir , goolZanak2 );
		}
		
	}
	
	//OwnRobot[def2].face ( Vec2 ( -side*2995 , 0 ) );
	//Navigate2Point ( def2 , Vec2 ( -side*1500 , 0 ) );
}