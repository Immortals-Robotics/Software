#include "../ai09.h"

void ai09::throwin_chip_shoot ( void )
{
	//swap ( mid2 , attack );
	//swap(attack, gk);
	
	GKHi(gk);
	DefHi(def);
	
	/*ERRTSetObstacles ( mid2 , true , true , true , true );
	OwnRobot[mid2].face(ball.Position);
	ERRTNavigate2Point ( mid2 , Vec2(side*500, -sgn(ball.Position.Y)*1500) ,0 , 70,&VELOCITY_PROFILE_MAMOOLI);
	
	ERRTSetObstacles ( mid1 , true , true , true , true );
	OwnRobot[mid1].face(ball.Position);
	ERRTNavigate2Point ( mid1 , Vec2(-side*500, -sgn(ball.Position.Y)*1800) ,0 , 70,&VELOCITY_PROFILE_MAMOOLI);*/
	
	ERRTSetObstacles ( mid2 , true , true , true , true );
    OwnRobot[mid2].face(ball.Position);
    ERRTNavigate2Point ( mid2 , Vec2(-side*1500, -sgn(ball.Position.Y)*2500.0f) ,0 , 70,&VELOCITY_PROFILE_MAMOOLI);
    oneTouchType[mid2] = shirje;
	
	ERRTSetObstacles ( mid1 , true , true , true , true );
	OwnRobot[mid1].face(ball.Position);
	ERRTNavigate2Point ( mid1 , Vec2(-side*3500, sgn(ball.Position.Y)*2500.0f) ,0 , 70,&VELOCITY_PROFILE_MAMOOLI);
    oneTouchType[mid1] = shirje;
	
	
	
	if (timer.time()>2 ) {
		//tech_circle(dmf,AngleWith ( Vec2 ( -side*2995 , 0 ) , ball.Position ) ,0,30,0,1);
		circle_ball(dmf, AngleWith ( Vec2 ( -side*field_width , sgn(ball.Position.Y)*2200.0f ) , ball.Position ), 0, 80, 1.0f);
	}
	else
	{
		//tech_circle(dmf,AngleWith ( Vec2 ( -side*2995 , 0 ) , ball.Position ) ,0,0,0,1);
		circle_ball(dmf, AngleWith ( Vec2 ( -side*field_width , 0 ) , ball.Position ), 0, 0, 1.0f);
	}
	
	OwnRobot[attack].face ( Vec2 ( -side*field_width , 0 ) );
	ERRTSetObstacles ( attack , 0,1,1 );
	AddCircle(ball.Position.X, ball.Position.Y, 320);
	if ( randomParam < 0.0 )
		ERRTNavigate2Point ( attack , PointOnConnectingLine ( ball.Position , Vec2 ( -side*field_width , 0 ) , 350+700*reached ) );
	else if ( randomParam < 0.5 )
		ERRTNavigate2Point ( attack , PointOnConnectingLine ( ball.Position , Vec2 ( -side*field_width , -sgn(ball.Position.X)*2000 ) , 350+700*reached ) );
	else
		ERRTNavigate2Point ( attack , PointOnConnectingLine ( ball.Position , Vec2 ( -side*field_width , sgn(ball.Position.X)*2000 ) , 350+700*reached ) );
	
	//swap(attack, gk);
	//swap ( mid2 , attack );
}