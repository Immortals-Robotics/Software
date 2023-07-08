#include "../ai09.h"

static float t_nml;

void ai09::penalty_us_ghuz ( void )
{
	bool canKickBall = timer.time() > 4;// bool(currentPlayParam);
	if (!canKickBall) {
		t_nml = timer.time();
	}
	GKHi(gk);
	DefMid(def, rw, lw, NULL, true);
	
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
	ERRTSetObstacles ( dmf, true, true, true, true );
	ERRTNavigate2Point ( dmf , Vec2 ( side * 5000 , 0 ), false, 80, &VELOCITY_PROFILE_AROOM );
	
	OwnRobot[mid1].face ( Vec2 ( -side*field_width , 0 ) );
	ERRTSetObstacles ( mid1, true, true, true, true );
	ERRTNavigate2Point ( mid1 , Vec2 ( -side*1000 , -500 ), false, 80, &VELOCITY_PROFILE_AROOM );
	
	OwnRobot[mid2].face ( Vec2 ( -side*field_width , 0 ) );
	ERRTSetObstacles ( mid2, true, true, true, true );
	ERRTNavigate2Point ( mid2 , Vec2 ( -side*1000 , 500 ), false, 80, &VELOCITY_PROFILE_AROOM );
	
	if ( ( canKickBall ))// && ( timer.time() > 1.0 ) )
	{
		PenaltyUs(attack, NormalizeAngle( penaltyAngle), 30 );
		//backPass(attack, NormalizeAngle( penaltyAngle),t_nml);
	}
	else
	{
		//PenaltyUs(attack,90-sgn(side)*90 ,0);
		
		TVec2 shootPoint;
		
        float shootDelta = (goal_width/2.0) - 50.0f;
        
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
		std::cout << "	GK e harif: " << oppGK << std::endl;
		if ( oppGK != -1 )
		{
			Line shootLine = Line::makeLineFromTwoPoints ( VecPosition ( shootPoint.X,shootPoint.Y ) , VecPosition ( ball.Position.X,ball.Position.Y ) );
			if (shootLine.getCircleIntersectionPoints(Circle(VecPosition(OppRobot[oppGK].Position.X,OppRobot[oppGK].Position.Y),130),intPoint,intPoint+1)>0) {
				shootPoint.Y = -shootPoint.Y;
			}
		}
				//penaltyAngle+=180;
		penaltyAngle = AngleWith ( shootPoint , ball.Position );

//        std::cout<<"__SHOOT_POINT_XY__"<<shootPoint.X<<"_"<<shootPoint.Y<<std::endl;
		
	}
}

void ai09::penalty_us_shootout(){

        bool canKickBall = bool(currentPlayParam);
        if (!canKickBall) {
        }

        DefMid(def, rw, lw, NULL, true);

        OwnRobot[dmf].face ( Vec2 ( -side*field_width , 0 ) );
        ERRTSetObstacles ( dmf, true, true, true, true );
        ERRTNavigate2Point ( dmf , Vec2 ( side * 4000 , 0 ), false, 80, &VELOCITY_PROFILE_AROOM );

        OwnRobot[mid1].face ( Vec2 ( -side*field_width , 0 ) );
        ERRTSetObstacles ( mid1, true, true, true, true );
        ERRTNavigate2Point ( mid1 , Vec2 ( side * 4000 , -500 ), false, 80, &VELOCITY_PROFILE_AROOM );

        OwnRobot[mid2].face ( Vec2 ( -side*field_width , 0 ) );
        ERRTSetObstacles ( mid2, true, true, true, true );
        ERRTNavigate2Point ( mid2 , Vec2 ( side * 4000 , 500 ), false, 80, &VELOCITY_PROFILE_AROOM );

        if (!canKickBall) {
                LOG_INFO("step0 - Waiting for permission");
        }else if(DIS(ball.Position, Vec2(-side*field_width,0)) >3000 ){
                circle_ball(attack, AngleWith(ball.Position, Vec2(-field_width, 0)), 1, 0, 0);
                LOG_INFO("step1 - Moving forward - waiting to get close to the opp goal");
        }else{
                circle_ball(attack, AngleWith(ball.Position, Vec2(-field_width, 400)), 60, 0, 0);
                LOG_INFO("step2 - Kick in the goal!!!!");
        }

}