#include "../ai09.h"

static float t_nml;

void ai09::penalty_us_ghuz ( void )
{
	bool canKickBall = bool(currentPlayParam);
	if (!canKickBall) {
		t_nml = timer.time();
	}
	GKHi(gk);
	DefHi(def);	
	
	/*side=-side;
	{
		float gkp_y = Line::makeLineFromTwoPoints ( VecPosition ( OwnRobot[attack].State.Position.X , OwnRobot[attack].State.Position.Y ) , VecPosition ( ball.Position.X , ball.Position.Y ) ).getIntersection ( Line::makeLineFromTwoPoints ( VecPosition ( side * 2940 , 100 ) , VecPosition ( side * 2940 , -100 ) ) ).getY ( );
		if(gkp_y > 250)
			gkp_y = 250;
		if(gkp_y < -250)
			gkp_y = -250;
		OwnRobot[gk].face(ball.Position);
		Navigate2Point ( gk , Vec2 ( side * 2940 , gkp_y ) );
	}
	side=-side;*/
	
	OwnRobot[dmf].face ( Vec2 ( -side*field_width , 0 ) );
	ERRTSetObstacles ( dmf );
	ERRTNavigate2Point ( dmf , Vec2 ( 0 , 0 ) );
	
	OwnRobot[mid1].face ( Vec2 ( -side*field_width , 0 ) );
	ERRTSetObstacles ( mid1 );
	ERRTNavigate2Point ( mid1 , Vec2 ( -side*1000 , -500 ) );
	
	OwnRobot[mid2].face ( Vec2 ( -side*field_width , 0 ) );
	ERRTSetObstacles ( mid2 );
	ERRTNavigate2Point ( mid2 , Vec2 ( -side*1000 , 500 ) );
	
	if ( ( canKickBall ) && ( timer.time() > 1.0 ) )
	{
		PenaltyUs(attack, NormalizeAngle( penaltyAngle), 80 );
		//backPass(attack, NormalizeAngle( penaltyAngle),t_nml);
	}
	else
	{
		//PenaltyUs(attack,90-sgn(side)*90 ,0);
		
		TVec2 shootPoint;
		
        float shootDelta = (goal_width/2.0) - 100.0f;
        
		if ( randomParam < 0.5 )
		{
			shootPoint = Vec2 ( -side*field_width,-shootDelta );
		}
		else {
			shootPoint = Vec2 ( -side*field_width,shootDelta );
		}

		//PenaltyUs(attack,AngleWith ( Vec2 ( -side*2995,-220 ) , ball.Position ) ,0);
		PenaltyUs(attack,AngleWith ( shootPoint , ball.Position ) ,0);
		
		VecPosition intPoint[2];
		side = -side;
		int oppGK = findCruncherOpp(-1, -1,true);
		side = -side;
		cout << "	GK e harif: " << oppGK << endl;
		if ( oppGK != -1 )
		{
			Line shootLine = Line::makeLineFromTwoPoints ( VecPosition ( shootPoint.X,shootPoint.Y ) , VecPosition ( ball.Position.X,ball.Position.Y ) );
			if (shootLine.getCircleIntersectionPoints(Circle(VecPosition(OppRobot[oppGK].Position.X,OppRobot[oppGK].Position.Y),130),intPoint,intPoint+1)>0) {
				shootPoint.Y = -shootPoint.Y;
			}
		}
				//penaltyAngle+=180;
		penaltyAngle = AngleWith ( shootPoint , ball.Position );
		
	}
}