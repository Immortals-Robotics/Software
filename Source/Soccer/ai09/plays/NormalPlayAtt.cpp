#include "../ai09.h"

Timer activeShootTimer;

void ai09::NormalPlayAtt ( void )
{
	ManageAttRoles ( );
	
	debugDraw=true;
	recievePass(dmf,PointOnConnectingLine(ball.Position, Vec2(side*field_width, 0), 2500));
	debugDraw=false;
	
	if (oneTouchType[attack]==allaf) {
		ERRTSetObstacles ( attack , false , true , true , true );
		OwnRobot[attack].face(Vec2(-side*field_width, 0));
		//OwnRobot[robot_num].target.Angle=-90;
		ERRTNavigate2Point ( attack , allafPos[attack] ,0 , 100,&VELOCITY_PROFILE_MAMOOLI);
		if (timer.time()>2.5) {
			oneTouchType[attack] = oneTouch;
		}
        
        activeShootTimer.start();
	}
    
    else
    {
        float ballReachTimeTmp = calculateBallRobotReachTime(attack, &VELOCITY_PROFILE_MAMOOLI) * 1.5;
        TVec2 ballReachPlace = predictBallForwardAI(ballReachTimeTmp);
        float ballGoalDot = (Dot(Normalize(Vec2(ball.velocity.x, ball.velocity.y)), Normalize(Vec2(-side*field_width, 0)-ballReachPlace)));
        if ( 0)//ballGoalDot > -0.6 && ballGoalDot < 0.7 && ball.velocity.magnitude > 900 )
        {
            float passAngle = ball.velocity.direction;
            tech_circle(attack, passAngle, 1, 0, 1, 0, 0, 1);
        }
        else
        {
            TVec2 openAngle = calculateOpenAngleToGoal(ball.Position, attack);
            
            bool mid1Reached = OwnRobot[mid1].State.velocity.magnitude < 50;
            bool mid2Reached = OwnRobot[mid2].State.velocity.magnitude < 50;
            
            bool mid1DisOk = DIS(OwnRobot[mid1].State.Position, ball.Position) > 2000;
            bool mid2DisOk = DIS(OwnRobot[mid2].State.Position, ball.Position) > 2000;
            
            bool mid1Seen = OwnRobot[mid1].State.seenState != CompletelyOut;
            bool mid2Seen = OwnRobot[mid2].State.seenState != CompletelyOut;
            
            bool mid1Suitable = mid1Seen && mid1Reached && mid1DisOk;
            bool mid2Suitable = mid2Seen && mid2Reached && mid2DisOk;
            
            if (mid1Suitable && mid2Suitable)
            {
                if(-side*OwnRobot[mid1].State.Position.X > -side*OwnRobot[mid2].State.Position.X)
                    mid2Suitable = false;
                else
                    mid1Suitable = false;
            }

	if ( openAngle.Y < 2 && (mid1Suitable||mid2Suitable) && (findKickerOpp(-1)==-1) )//&& ( ball.Position.X * side < -2300 ) && ( fabs ( ball.Position.Y ) > 1800 ) )
	{
        
		//float passAngle = AngleWith ( OwnRobot[randomParam<0.3?dmf:(randomParam<0.6?rmf:lmf)].State.Position , ball.Position );
        float passAngle = AngleWith ( Vec2 ( -side*1700 , -sgn ( ball.Position.Y ) * 1700 ) , ball.Position );
        float chip_pow = 40;
        
        if ( mid1Suitable )
        {
            passAngle = AngleWith ( OwnRobot[mid1].State.Position , ball.Position );
            chip_pow = 50.0 * DIS(OwnRobot[mid1].State.Position, ball.Position) / 2000.0f;
        }
        else if ( mid2Suitable )
        {
            passAngle = AngleWith ( OwnRobot[mid2].State.Position , ball.Position );
            chip_pow = 50.0 * DIS(OwnRobot[mid2].State.Position, ball.Position) / 2000.0f;
        }
        else
        {
            passAngle = AngleWith ( Vec2 ( -side*1700 , -sgn ( ball.Position.Y ) * 1700 ) , ball.Position );
            chip_pow = 1;
        }
		tech_circle(attack, passAngle, 0, chip_pow, 1, 0, 0, 1);
	}
	else {
        
        float shootAngle;
        if ( openAngle.Y > 10 )
            shootAngle = NormalizeAngle( 180+openAngle.X);
        else
            shootAngle = AngleWith ( Vec2 ( -side*field_width , 0 ) , ball.Position );
        
		float shoot_pow = 80 - OwnRobot[attack].State.velocity.magnitude * 0.01;


        //if ( openAngle.Y < 2 )
        //    shoot_pow = 0;
		if (DIS(OwnRobot[attack].State.Position,ball.Position) > 400 ) {
			shoot_pow = 1;
            activeShootTimer.start();
		}
		else if (goal_blocked(ball.Position, 200, 90)) {
			shoot_pow = 1;
		}
        else
        {
            //if ( activeShootTimer.time()<0.3 )
            //{
            //    shoot_pow = 1;
            //}
        }
		
		//if (attackFuckingAngle()) {
		//	shootAngle = AngleWith(ball.Position, Vec2(side*field_width, 0));
		//	shoot_pow = 1;
		//}
		
		debugDraw = true;
		tech_circle(attack, shootAngle, shoot_pow, 0, 1, 0, 0, 0);
		//circle_ball(attack, 90, 80, 0, 1.0f);
		debugDraw = false;
	}
        
        }
        
    }

	if (ball.Position.Y>600) {
		recievePass(mid1, Vec2 ( -side*250 , 0 ));
	}
	else {
		recievePass(mid1, Vec2 ( -side*(field_width-800) , field_height-800 ));
	}
	
	if (ball.Position.Y<-600) {
		recievePass(mid2, Vec2 ( -side*250 , 0 ));
	}
	else {
		recievePass(mid2, Vec2 ( -side*(field_width-800) ,-field_height+800 ));
	}

}