#include "../ai09.h"

void ai09::kickoff_us_zamini ( void )
{
	bool canKickBall = bool(currentPlayParam);
	GKHi(gk, 1, 0);
	DefHi(def);
	
	ERRTSetObstacles ( dmf , true , true , true , true );
	OwnRobot[dmf].face(ball.Position);
	ERRTNavigate2Point ( dmf , PointOnConnectingLine(ball.Position, Vec2(side*3025, 0), DIS(ball.Position, Vec2(side*3025, 0))/3.0f) ,0 , 40,&VELOCITY_PROFILE_MAMOOLI);
	
	float waitTime = 1.0f;
	
	int shootSide = 1;
	if (randomParam<1.5) {
		shootSide = -1;
	}
	
	if ( canKickBall )
	{
		OwnRobot[attack].face ( Vec2 ( -side*2995 , 0 ) );
		OwnRobot[lmf].face ( Vec2 ( -side*2995 , 0 ) );
		
		if ( timer.time() < waitTime*2 )
		{
			ERRTSetObstacles(attack, 0, 1, 1, 1);
			ERRTNavigate2Point ( attack , Vec2 ( side*100 , shootSide*1800 ) );
			ERRTSetObstacles(lmf, 1, 1, 1, 1);
			ERRTNavigate2Point ( lmf , Vec2 ( side*100 , -shootSide*1800 ) );
			tech_circle(rmf,AngleWith ( Vec2 ( -side*2995,shootSide*4000 ) , ball.Position ), 0,0,0,1,0,0);
		}
		else if ( timer.time() < waitTime * 2.0f )
		{
			ERRTSetObstacles(attack, 0, 1, 1, 1);
			ERRTNavigate2Point ( attack , Vec2 ( side*100 , shootSide*1800 ) );
			ERRTSetObstacles(lmf, 1, 1, 1, 1);
			ERRTNavigate2Point ( lmf , Vec2 ( side*100 , -shootSide*1800 ) );
			tech_circle(rmf,AngleWith ( Vec2 ( -side*2995,shootSide*4000 ) , ball.Position ), 0,0,0,1,1,0);
		}
		else {
			ERRTSetObstacles(attack, 0, 1, 1, 1);
			ERRTNavigate2Point ( attack , Vec2 ( side*100 , shootSide*1800 ) );
			ERRTSetObstacles(lmf, 1, 1, 1, 1);
			ERRTNavigate2Point ( lmf , Vec2 ( side*100 , -shootSide*1800 ) );
			tech_circle(rmf,AngleWith ( Vec2 ( -side*2995,shootSide*4000 ) , ball.Position ), 6,0,0,1,0,0);
		}
	}
	else
	{
		tech_circle(rmf,AngleWith ( Vec2 ( -side*2995,shootSide*2000 ) , ball.Position ),0,0,0,1,0,0);
		
		OwnRobot[lmf].face ( Vec2 ( -side*2995 , 0 ) );
		ERRTSetObstacles(lmf, 1, 1, 1, 1);
		ERRTNavigate2Point ( lmf , Vec2 ( side*100 , -shootSide*1600 ) );		
		
		OwnRobot[attack].face ( Vec2 ( -side*2995 , 0 ) );
		ERRTSetObstacles(attack, 1, 1, 1, 1);
		ERRTNavigate2Point ( attack , Vec2 ( side*100 , shootSide*1600 ) );		
	}
	
	//swap(attack, passgir);
}