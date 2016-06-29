#include "../ai09.h"

int lockAngleCounter = 0;
int elendil = 0;
TVec2 Pelendil;
TVec2 ai09::predictBallForwardAI( float timeAhead )
{
	BallState _ball = ball;
	_ball.Position.X /= (float)1000.0;
	_ball.Position.Y /= (float)1000.0;
	_ball.velocity.x /= (float)1000.0;
	_ball.velocity.y /= (float)1000.0;
	float k = 0.25f; //velocity derate every sec(units (m/s)/s)
	//float frame_rate = 61.0f;
	//float tsample = (float)1.0f/(float)frame_rate;
	
	float vx_vision = _ball.velocity.x; 
	float vy_vision = _ball.velocity.y;
	
	float xpos_vision = _ball.Position.X;
	float ypos_vision = _ball.Position.Y;
	
	float vball_vision = float(sqrt(vx_vision*vx_vision + vy_vision*vy_vision));
	
	float t = timeAhead;
	
	float v = vball_vision - k*t;
	float dist0 = vball_vision*t - k*(t*t)/2.0f;
	
	float dist;
	float vball_pred;
	
	// if speed turns out to be negative..it means that ball has stopped, so calculate that amount of 
	// distance traveled
	if(v < 0){
		vball_pred = 0.0f;
		dist = (vball_vision*vball_vision)*k/2.0f;
		// i.e the ball has stopped, so take a newer vision data for the prediction
	}
	else{
		vball_pred = v;
		dist = dist0;
	}
	
	if(vball_vision != 0){
		_ball.velocity.x = vball_pred*(vx_vision)/vball_vision;
		_ball.velocity.y = vball_pred*(vy_vision)/vball_vision;
		_ball.Position.X = (xpos_vision + dist*(vx_vision)/vball_vision);
		_ball.Position.Y = (ypos_vision + dist*(vy_vision)/vball_vision);
	}
	
	_ball.velocity.x *= (float) 1000.0;
	_ball.velocity.y *= (float) 1000.0;
	_ball.Position.X *= (float) 1000.0;
	_ball.Position.Y *= (float) 1000.0;
	
	return _ball.Position;
}

float ai09::calculateRobotReachTime ( int robot_num , TVec2 dest , VelocityProfile* vel_profile )
{
	return DIS(OwnRobot[robot_num].State.Position, dest) / (Magnitude(vel_profile->max_spd)*42.5*0.5); 
}

float ai09::calculateBallRobotReachTime ( int robot_num , VelocityProfile* vel_profile )
{
    static MedianFilter<float> predTFilt(5);
    
    const float tMax = 5.0;
    float predT = tMax;
    for (float forwT = 0; forwT < tMax ; forwT += 0.02f ) {
        TVec2 newBallPos = predictBallForwardAI ( forwT );
        float robotReachT = calculateRobotReachTime(robot_num, newBallPos, vel_profile);
        if ( robotReachT <= forwT )
        {
            predT = robotReachT;
            break;
        }
    }
    predTFilt.AddData(predT);
    
    return predTFilt.GetCurrent();
}

void ai09::tech_circle ( int robot_num , float angle , int kick , int chip , bool needRRT , bool gameRestart , bool kiss , bool dribbler , bool needOppRRT)
{	
	//kick=100;
	if (gameRestart&&(chip>0)) {
		chip_head = OwnRobot[robot_num].State.Angle;
	}
	
	
	
	if (1)// ( fabs(NormalizeAngle(lastBallDirection-ball.velocity.direction) ) > 5 ) || ( fabs(lastBallMagnitude-ball.velocity.magnitude) > 80 ) || ( ball.velocity.magnitude < 100 ) || ( DIS(ball.Position, PredictedBall) < 150) )
	{
		lastBallDirection = ball.velocity.direction;
		lastBallMagnitude = ball.velocity.magnitude;
		
		PredictedBall = ball.Position;
		
		
		
		//PredictedBall = predictBallForwardAI(calculateBallRobotReachTime(robot_num,&VELOCITY_PROFILE_MAMOOLI));
		if (1)// ( fabs(NormalizeAngle(lastBallDirection-ball.velocity.direction) ) > 5 ) || ( fabs(lastBallMagnitude-ball.velocity.magnitude) > 80 ) || ( ball.velocity.magnitude < 100 ) || ( DIS(ball.Position, PredictedBall) < 150) )
		{
			float d = DIS ( OwnRobot[robot_num].State.Position , CircleAroundPoint ( ball.Position , angle , 200.0f ) );
			d += DIS ( ball.Position , CircleAroundPoint ( ball.Position , angle , 200.0f ) );
			d -= 100;
			
			//float d = DIS ( OwnRobot[robot_num].State.Position , ball.Position );
			
			d /= 2000.0f;
			//d = 0.0f;
			
			//d *= 50.0f;
			
			//d = min ( 20.0f , d );
			
			//d += 5.0f;
			
			//d = 0.0f;
			
			PredictedBall.X += d * ball.velocity.x / 45.0f;
			PredictedBall.Y += d * ball.velocity.y / 45.0f;
			
			PredictedBall = predictBallForwardAI(d);
			
		}
		
		AddDebugCircle(PredictedBall,100,Blue);
		
		/*float d = DIS ( OwnRobot[robot_num].State.Position , CircleAroundPoint ( ball.Position , angle , 200.0f ) );
		d += DIS ( ball.Position , CircleAroundPoint ( ball.Position , angle , 200.0f ) );
		d -= 100;
		
		//float d = DIS ( OwnRobot[robot_num].State.Position , ball.Position );
		
		d /= 2000.0f;
		//d = 0.0f;
		
		//d *= 50.0f;
		
		//d = min ( 20.0f , d );
		
		//d += 5.0f;
		
		//d = 0.0f;
		
		PredictedBall.X += d * ball.velocity.x / 45.0f;
		PredictedBall.Y += d * ball.velocity.y / 45.0f;
		
		PredictedBall = predictBallForwardAI(d);*/

	}
	
	static bool passedBall = false;
	
	float prepPredictMul = 0.15;
	float goalPredictMul = 0.0;

	if ( passedBall )
	{
		prepPredictMul = 0.0;
		goalPredictMul = -0.15;
	}
	
	TVec2 ballToGoal = Vec2(-side*field_width, 0) - ball.Position;
	ballToGoal = Normalize(ballToGoal);
	float ballVelToGoalDot = (ball.velocity.x*ballToGoal.X+ball.velocity.y*ballToGoal.Y);
	TVec2 ballVelToGoal = Vec2(ballToGoal.X*ballVelToGoalDot, ballToGoal.Y*ballVelToGoalDot);
	TVec2 ballVelPrepToGoal = Vec2(ball.velocity.x-ballVelToGoal.X, ball.velocity.y-ballVelToGoal.Y);
	
	//AddDebugLine(ball.Position,ball.Position+Vec2(ball.velocity.x, ball.velocity.y)/1.0f);
	//AddDebugLine(ball.Position,ball.Position+ballVelToGoal/1.0f , Pink);
	//AddDebugLine(ball.Position,ball.Position+ballVelPrepToGoal/1.0f , Green);
	
	//PredictedBall += ballVelPrepToGoal * prepPredictMul;
	//PredictedBall += ballVelToGoal * goalPredictMul;
	
	//AddDebugCircle(PredictedBall, 70, Purple);
	
	TVec2 robotToBall = OwnRobot[robot_num].State.Position - ball.Position;
	float robotToBallDot = robotToBall.X*ballVelPrepToGoal.X+robotToBall.Y*ballVelPrepToGoal.Y;
	robotToBallDot /= Magnitude(robotToBall);
	robotToBallDot /= Magnitude(ballVelPrepToGoal);
	//if ( !passedBall )
	//{
		//if ( DIS(PredictedBall, OwnRobot[robot_num].State.Position) < 250 )
		if ( robotToBallDot > 0.3 )
			passedBall = true;
	//}
	//else if ( DIS(ball.Position, OwnRobot[robot_num].State.Position) > 750 )
	else if ( ( robotToBallDot < -0.2 ) || ( DIS(ball.Position, OwnRobot[robot_num].State.Position) > 1050 ) )
		passedBall = false;
	
	if (passedBall) {
		AddDebugCircle(OwnRobot[robot_num].State.Position, 200, Gold);
	}
	
	if ( DIS(ball.Position, OwnRobot[robot_num].State.Position) < 400 )
		circleReachedBehindBall = true;
	else if ( DIS(ball.Position, OwnRobot[robot_num].State.Position) > 600 )
		circleReachedBehindBall = false;

	if ( 0 )
	{
		ERRTSetObstacles ( robot_num , 0 , 0 , 1 , 1 , 0 );
		planner[robot_num].init ( ball.Position , Vec2 ( -0 , 0 ) , 9 );
		TVec2 wayp = planner[robot_num].Plan ( );

		//wayp = Vec2 ( -3025 , 0 );

		angle = AngleWith ( wayp , ball.Position );
	}

	angle = NormalizeAngle ( angle );

	//cout << "circle dadam";


	
	float r = 150.0f;
	
	if (kiss) {
		r = 82.0f;
	}

	float tetta = 10.0f;

	if ( ( kick > 0 ) || ( chip > 0 ) )
	{
		if ( gameRestart )
		{
			r = 200.0f;
			tetta =20.0f;
		}
		else
		{
			r = 400.0f;
			tetta = 32.0f;
			//if ( !passedBall )
			//	r *= 1.5;
		}
	}

	//dribbler=0;
	if ( dribbler )
	{
		if ( DIS ( OwnRobot[robot_num].State.Position , ball.Position ) < 190 )
			OwnRobot[robot_num].Dribble ( 15 );
	}

	if ( gameRestart )
		OwnRobot[robot_num].face ( PredictedBall );
	else
		OwnRobot[robot_num].target.Angle = NormalizeAngle ( angle - 180.0f );

	float hehe = AngleWith ( PredictedBall , OwnRobot[robot_num].State.Position );
	hehe = NormalizeAngle ( angle - hehe );
	

	if ( needRRT )
    {
        needOppRRT = (OwnRobot[robot_num].State.velocity.magnitude > 600) && (DIS(OwnRobot[robot_num].State.Position,ball.Position)>300);
		ERRTSetObstacles ( robot_num , false , true , true , needOppRRT , false , false );
    }

	else
		clear_map ( );
	
	if ( ( fabs ( hehe ) < tetta ) )//|| ( circleReachedBehindBall ) )
	{
        
		AddDebugCircle(Vec2(0, 0),1000,Red);
		//hehe = angle;
		//if ( OwnRobot[2].State.Angle < 0 )
		if (( kick) ||(chip ))
		{
            
			if ( circleReachedBehindBall )
			{
				//cout << "								reached	";
				TVec2 targetPoint;
				if ( !gameRestart )
				{
					targetPoint = CircleAroundPoint ( PredictedBall , angle , min ( (r/1.6) , fabs(hehe)*320.0f/(tetta*1.2) ) );
				}
				else
					targetPoint = CircleAroundPoint ( PredictedBall , angle , min ( r , fabs(hehe)*320.0f/(tetta) ) );

				//Line newLine = Line::makeLineFromTwoPoints ( VecPosition ( targetPoint.X , targetPoint.Y ) , VecPosition ( OwnRobot[robot_num].State.Position.X , OwnRobot[robot_num].State.Position.Y ) );
				//Circle newCircle ( VecPosition ( OwnRobot[robot_num].State.Position.X , OwnRobot[robot_num].State.Position.Y ) , DIS(targetPoint, OwnRobot.State.Position) * 2 );
				if ( 1)//!gameRestart )
				{
					//cout << "elendil: " << elendil;
					float hehe2 = AngleWith ( PredictedBall , OwnRobot[robot_num].State.Position );
					hehe2 = NormalizeAngle ( angle - hehe2 );
					bool el = ((hehe2<5)&&(DIS(ball.Position,OwnRobot[robot_num].State.Position)<100));
					if ( el || ( elendil > 0 ) )
					{
						
						targetPoint.X-=150.0*cosDeg(angle);
						targetPoint.Y-=150.0*sinDeg(angle);
						//targetPoint = Vec2(3*targetPoint.X-2*OwnRobot[robot_num].State.Position.X, 3*targetPoint.Y-2*OwnRobot[robot_num].State.Position.Y);
						targetPoint.X /= 1;
						targetPoint.Y /= 1;
						
						//if (elendil<=0)
						{
							Pelendil=targetPoint;
						}
						
						elendil--;
						if (el) {
							elendil = 30;
						}
						ERRTNavigate2Point ( robot_num , Pelendil,0,100,&VELOCITY_PROFILE_KHARAKI );
					}
					else
					{
                        //targetPoint.X -= 550.0 * pow(fabs ( sinDeg(hehe) ),4.0) * cosDeg(angle);
                        //targetPoint.Y -= 550.0 * pow(fabs ( sinDeg(hehe) ),4.0) * sinDeg(angle);
						targetPoint = Vec2(3*targetPoint.X-OwnRobot[robot_num].State.Position.X, 3*targetPoint.Y-OwnRobot[robot_num].State.Position.Y);
						targetPoint.X /= 2;
						targetPoint.Y /= 2;
						ERRTNavigate2Point ( robot_num , targetPoint,0,100,&VELOCITY_PROFILE_KHARAKI );

					}
					//Halt(robot_num);
				}
				
				else {
					ERRTNavigate2Point ( robot_num , targetPoint,0,100,&VELOCITY_PROFILE_MAMOOLI );
				}


				//targetPoint = Vec2((targetPoint.X+OwnRobot[robot_num].State.Position.X)/2, (targetPoint.Y+OwnRobot[robot_num].State.Position.Y)/2);
							}
			else
				ERRTNavigate2Point ( robot_num , CircleAroundPoint ( PredictedBall , angle , min ( r , fabs(hehe)*320.0f/tetta ) ),0,100,&VELOCITY_PROFILE_MAMOOLI );
			//circleReachedBehindBall = true;
		}
		else
			ERRTNavigate2Point ( robot_num , CircleAroundPoint ( PredictedBall , angle+hehe*0.0f , r ) , 0 ,100);
	}
	else
	{
		AddDebugCircle(Vec2(0, 0),1000,Orange);
		

        
		hehe = AngleWith ( PredictedBall , OwnRobot[robot_num].State.Position ) + sgn ( hehe ) * tetta;
		//if ( DIS ( OwnRobot[robot_num].State.Position , ball.Position ) < 200 )
		//	ERRTNavigate2Point ( robot_num , CircleAroundPoint ( PredictedBall , hehe , r ) , 0 , 20 );
		//else
		if ( passedBall )
			ERRTNavigate2Point ( robot_num , CircleAroundPoint ( PredictedBall , hehe , r ) , 0 , 100 );
		else
			ERRTNavigate2Point ( robot_num , CircleAroundPoint ( PredictedBall , hehe , r ) , 0 , 100 , &VELOCITY_PROFILE_KHARAKI );

	}
	
	AddDebugCircle(OwnRobot[robot_num].State.Position,90,Blue_Violet);
	AddDebugLine(OwnRobot[robot_num].State.Position,OwnRobot[robot_num].State.Position+Normalize(OwnRobot[robot_num].target.Position-OwnRobot[robot_num].State.Position)*1000, Black);


	if ( ( kick > 0 ) || ( chip > 0 ) )
	{
		TVec2 tmpPos = OwnRobot[robot_num].State.Position;
		float tmpAng = OwnRobot[robot_num].State.Angle;
		/*if ( side == -1 )
		{
			tmpPos.X = -tmpPos.X;
			tmpAng += 180.0f;
		}*/
		float t1 = AngleWith ( tmpPos , Vec2 ( field_width , -goal_width/2.0 ) );
		float t2 = AngleWith ( tmpPos , Vec2 ( field_width , goal_width/2.0 ) );

		while ( t1 < 0 )
			t1 += 360.0f;
		while ( t1 > 360 )
			t1 -= 360.0f;
		while ( t2 < 0 )
			t2 += 360.0f;
		while ( t2 > 360 )
			t2 -= 360.0f;

		float ot = tmpAng;
		while ( ot < 0 )
			ot += 360.0f;
		while ( ot > 360 )
			ot -= 360.0f;

		//if ( ( ot > t2 ) && ( ot < t1 ) )
		//if ( fabs ( NormalizeAngle ( OwnRobot[robot_num].State.Angle - AngleWith ( OwnRobot[robot_num].State.Position , Vec2 ( -3025 , 0 ) ) ) ) < 20 ) 
		//if ( ( ( side == -1 ) && ( fabs ( OwnRobot[robot_num].State.Angle ) < 140 ) ) ||
		//	 ( ( side == 1 ) && ( fabs ( OwnRobot[robot_num].State.Angle ) > 40 ) ) )
		if ( fabs ( NormalizeAngle ( OwnRobot[robot_num].target.Angle - OwnRobot[robot_num].State.Angle ) ) < 40 )
			lockAngleCounter ++;
		else
			lockAngleCounter = 0;

		//if ( lockAngleCounter > 1 )
		{
			//if ( chip )
			OwnRobot[robot_num].Chip(chip);
			//else
            kick = min(80, kick);
            float vel_delta = OwnRobot[robot_num].State.velocity.magnitude / 100.0f;
            vel_delta = min(40,vel_delta);
            vel_delta = kick - vel_delta;
			OwnRobot[robot_num].Shoot(kick);
		}
	}
}