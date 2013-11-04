#include "ai09.h"

void ai09::penalty_us_ghuz ( bool canKickBall )
{
	GK_Ghuz(gk);
	DefGhuz(def);	
	
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
	
	OwnRobot[dmf].face ( Vec2 ( -side*2995 , 0 ) );
	ERRTSetObstacles ( dmf );
	ERRTNavigate2Point ( dmf , Vec2 ( 0 , 0 ) );
	
	OwnRobot[lmf].face ( Vec2 ( -side*2995 , 0 ) );
	ERRTSetObstacles ( lmf );
	ERRTNavigate2Point ( lmf , Vec2 ( -side*1000 , -500 ) );
	
	OwnRobot[rmf].face ( Vec2 ( -side*2995 , 0 ) );
	ERRTSetObstacles ( rmf );
	ERRTNavigate2Point ( rmf , Vec2 ( -side*1000 , 500 ) );
	
	if ( canKickBall )
	{
		PenaltyUs(attack, NormalizeAngle( penaltyAngle), 15 );
	}
	else
	{
		//PenaltyUs(attack,90-sgn(side)*90 ,0);
		
		TVec2 shootPoint;
		
		if ( randomParam < 0.5 )
		{
			shootPoint = Vec2 ( -side*2995,-250 );
		}
		else {
			shootPoint = Vec2 ( -side*2995,250 );
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