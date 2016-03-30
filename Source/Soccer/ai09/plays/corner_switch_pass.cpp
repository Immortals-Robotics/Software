#include "../ai09.h"

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
		OneDef(def);
		//Halt(dmf);
		Halt(lmf);
		
		OwnRobot[dmf].face(Vec2(-side*3025, 0));
		ERRTSetObstacles(dmf, 1, 1, 1, 1,0);
		ERRTNavigate2Point(dmf, Vec2(side*800, sgn(ball.Position.Y)*1400), 0, 100);
		
		if ( timer.time() < 3.5 )
		{
			ERRTSetObstacles ( attack );
			tech_circle(attack,AngleWith ( goolZanak1 , ball.Position ) ,0,0);
		}
		else
		{
			ERRTSetObstacles ( attack );
			tech_circle(attack,AngleWith ( goolZanak2 , ball.Position ) ,6,0,0,1);
		}
		
		//reached = true;
	}
	else
	{
		GK ( gk , 1 );
		OneDef ( def );
		
		Halt(dmf);
		Halt(lmf);
		
		ERRTSetObstacles ( attack );
		tech_circle(attack,90-side*90 ,0,0);

		if ( timer.time() > 1.5 )
		{
			//if ( !reached )
			//swap(attack, passgir);
			int hehet = OwnRobot[rmf].vision_id;
			OwnRobot[rmf].set_vision_id(OwnRobot[dmf].vision_id);
			OwnRobot[dmf].set_vision_id(hehet);
			reached = true;
		}
	}
	
	if ( oneTouchDetector[rmf].IsArriving(70) )
	{
		WaitForPass ( rmf );
	}
	else
	{
		hys = 0;
		OwnRobot[rmf].face ( Vec2 ( -side*2995 , 0 ) );
		ERRTSetObstacles ( rmf );
		if ( timer.time() < 1.5 )
		{
			ERRTNavigate2Point ( rmf , goolZanak1 );
		}
		else 
		{
			ERRTNavigate2Point ( rmf , goolZanak2 );
		}
		
	}
	
	//OwnRobot[def2].face ( Vec2 ( -side*2995 , 0 ) );
	//Navigate2Point ( def2 , Vec2 ( -side*1500 , 0 ) );
}