#include "ai09.h"

void ai09::throwin_chip_shoot ( void )
{
	swap ( rmf , attack );
	//swap(attack, gk);
	
	GK_Ghuz(gk);
	DefGhuz(def);
	
	ERRTSetObstacles ( dmf , true , true , true , true );
	OwnRobot[dmf].face(ball.Position);
	ERRTNavigate2Point ( dmf , Vec2(side*1500, 0) ,0 , 70,&VELOCITY_PROFILE_MAMOOLI);
	
	ERRTSetObstacles ( lmf , true , true , true , true );
	OwnRobot[lmf].face(ball.Position);
	ERRTNavigate2Point ( lmf , Vec2(-side*2300, -sgn(ball.Position.Y)*1500) ,0 , 70,&VELOCITY_PROFILE_MAMOOLI);
	
	
	
	if (timer.time()>2 ) {
		tech_circle(attack,AngleWith ( Vec2 ( -side*2995 , 0 ) , ball.Position ) ,0,6,0,1);
	}
	else
	{
		tech_circle(attack,AngleWith ( Vec2 ( -side*2995 , 0 ) , ball.Position ) ,0,0,0,1);
	}
	
	OwnRobot[rmf].face ( Vec2 ( -side*2995 , 0 ) );
	ERRTSetObstacles ( rmf , 0,1,1 );
	if ( randomParam < 0.3 )
		ERRTNavigate2Point ( rmf , PointOnConnectingLine ( ball.Position , Vec2 ( -side*2995 , 0 ) , 300+700*reached ) );
	else if ( randomParam < 0.6 )
		ERRTNavigate2Point ( rmf , PointOnConnectingLine ( ball.Position , Vec2 ( -side*2995 , -sgn(ball.Position.X)*2000 ) , 300+700*reached ) );
	else
		ERRTNavigate2Point ( rmf , PointOnConnectingLine ( ball.Position , Vec2 ( -side*2995 , sgn(ball.Position.X)*2000 ) , 300+700*reached ) );
	
	//swap(attack, gk);
	swap ( rmf , attack );
}