#include "../ai09.h"

void ai09::kickoff_us_farar ( void )
{
	bool canKickBall = bool(currentPlayParam);
	GKHi(gk, 1, 0);
	DefHi(def);

	ERRTSetObstacles ( dmf , true , true , true , true );
	OwnRobot[dmf].face(ball.Position);
	ERRTNavigate2Point ( dmf , PointOnConnectingLine(ball.Position, Vec2(side*field_width, 0), DIS(ball.Position, Vec2(side*field_width, 0))/3.0f) ,0 , 40,&VELOCITY_PROFILE_MAMOOLI);
	
	float waitTime = 0.7f;
	
	if ( canKickBall )
	{
		OwnRobot[attack].face ( Vec2 ( -side*field_width , 0 ) );
		OwnRobot[lmf].face ( Vec2 ( -side*field_width , 0 ) );

		if ( timer.time() < waitTime )
		{
			ERRTSetObstacles(attack, 0, 1, 1, 1);
			ERRTNavigate2Point ( attack , Vec2 ( side*100 , 2600 ) );
			ERRTSetObstacles(lmf, 1, 1, 1, 1);
			ERRTNavigate2Point ( lmf , Vec2 ( side*100 , -2600 ) );
			tech_circle(rmf,AngleWith ( Vec2 ( -side*field_width,3000 ) , ball.Position ), 0,0,0,1,0,0);
		}
		else if ( timer.time() < waitTime * 2.0f )
		{
			ERRTSetObstacles(attack, 0, 1, 1, 1);
			ERRTNavigate2Point ( attack , Vec2 ( side*100 , 1300 ) );
			ERRTSetObstacles(lmf, 1, 1, 1, 1);
			ERRTNavigate2Point ( lmf , Vec2 ( side*100 , -1300 ) );
			tech_circle(rmf,AngleWith ( Vec2 ( -side*field_width,3000 ) , ball.Position ), 0,0,0,1,0,0);
		}
		else {
			ERRTSetObstacles(attack, 0, 1, 1, 1);
			ERRTNavigate2Point ( attack , Vec2 ( side*100 , 1300 ) );
			ERRTSetObstacles(lmf, 1, 1, 1, 1);
			ERRTNavigate2Point ( lmf , Vec2 ( side*100 , -1300 ) );
			tech_circle(rmf,AngleWith ( Vec2 ( -side*field_width,3000 ) , ball.Position ), 0,90,0,1,0,0);
		}
	}
	else
	{
		tech_circle(rmf,AngleWith ( Vec2 ( -side*field_width,3000 ) , ball.Position ));

		OwnRobot[attack].face ( Vec2 ( -side*field_width , 0 ) );
		ERRTSetObstacles(attack, 1, 1, 1, 1);
		ERRTNavigate2Point ( attack , Vec2 ( side*100 , 2600 ) );
		
		OwnRobot[lmf].face ( Vec2 ( -side*field_width , 0 ) );
		ERRTSetObstacles(lmf, 1, 1, 1, 1);
        ERRTNavigate2Point ( lmf , Vec2 ( side*100 , (randomParam>0.5f)?-2600:2600 ) );
	}
}