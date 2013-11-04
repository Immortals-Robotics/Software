#include "ai09.h"

#include "helpers.h"

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


void ai09::tech_circle ( int robot_num , float angle , int kick , int chip , bool needRRT , bool gameRestart , bool kiss , bool dribbler )
{
	if (1)// ( fabs(NormalizeAngle(lastBallDirection-ball.velocity.direction) ) > 5 ) || ( fabs(lastBallMagnitude-ball.velocity.magnitude) > 80 ) || ( ball.velocity.magnitude < 100 ) || ( DIS(ball.Position, PredictedBall) < 150) )
	{
		lastBallDirection = ball.velocity.direction;
		lastBallMagnitude = ball.velocity.magnitude;
		
		Debug_Circle * dbgCircle = AIDebug.add_circle();
		dbgCircle -> set_x(ball.Position.X);
		dbgCircle -> set_y(ball.Position.Y);
		dbgCircle -> set_r(150);
		color * col = dbgCircle -> mutable_col();
		col -> set_r(255);
		col -> set_g(0);
		col -> set_b(0);
		
		PredictedBall = ball.Position;
		
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

		
	Debug_Circle * dbgCircle = AIDebug.add_circle();
	dbgCircle -> set_x(PredictedBall.X);
	dbgCircle -> set_y(PredictedBall.Y);
	dbgCircle -> set_r(70);
	color * col = dbgCircle -> mutable_col();
	col -> set_r(0);
	col -> set_g(255);
	col -> set_b(255);
	//cout << "circle dadam";


	
	float r = 150.0f;
	
	if (kiss) {
		r = 75.0f;
	}

	float tetta = 10.0f;

	if ( ( kick > 0 ) || ( chip > 0 ) )
	{
		if ( gameRestart )
		{
			r = 200.0f;
			tetta =25.0f;
		}
		else
		{
			r = 400.0f;
			tetta = 52.0f;
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
		ERRTSetObstacles ( robot_num );
	else
		clear_map ( );
	
	if ( ( fabs ( hehe ) < tetta ) )//|| ( circleReachedBehindBall ) )
	{
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
				if ( !gameRestart )
				{
					cout << "elendil: " << elendil;
					float hehe2 = AngleWith ( PredictedBall , OwnRobot[robot_num].State.Position );
					hehe2 = NormalizeAngle ( angle - hehe2 );
					bool el = ((hehe2<5)&&(DIS(ball.Position,OwnRobot[robot_num].State.Position)<100));
					if (0)// el || ( elendil > 0 ) )
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
		hehe = AngleWith ( PredictedBall , OwnRobot[robot_num].State.Position ) + sgn ( hehe ) * tetta;
		//if ( DIS ( OwnRobot[robot_num].State.Position , ball.Position ) < 200 )
		//	ERRTNavigate2Point ( robot_num , CircleAroundPoint ( PredictedBall , hehe , r ) , 0 , 20 );
		//else
			ERRTNavigate2Point ( robot_num , CircleAroundPoint ( PredictedBall , hehe , r ) , 0 , 100 );
	}

	if ( ( kick > 0 ) || ( chip > 0 ) )
	{
		TVec2 tmpPos = OwnRobot[robot_num].State.Position;
		float tmpAng = OwnRobot[robot_num].State.Angle;
		/*if ( side == -1 )
		{
			tmpPos.X = -tmpPos.X;
			tmpAng += 180.0f;
		}*/
		float t1 = AngleWith ( tmpPos , Vec2 ( -3025.0f , -350.0f ) );
		float t2 = AngleWith ( tmpPos , Vec2 ( -3025.0f , 350.0f ) );

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
			OwnRobot[robot_num].Shoot(kick);
		}
	}
}

/*void predictBallForward( float step )
{
	ball.Position.X /= (float)100.0;
	ball.Position.Y /= (float)100.0;
	ball.velocity.x /= (float)100.0;
	ball.velocity.y /= (float)100.0;
	float k = 0.25f; //velocity derate every sec(units (m/s)/s)
	float frame_rate = 61.0f;
	float tsample = (float)1.0f/(float)frame_rate;
	float frames_ahead = (float)61.0f/(float)step; //can be any real number theoretically
  
	float vx_vision = state -> ball.velocity.x; 
	float vy_vision = state -> ball.velocity.y;
  
	float xpos_vision = state -> ball.Position.X;
	float ypos_vision = state -> ball.Position.Y;
  
	float vball_vision = float(sqrt(vx_vision*vx_vision + vy_vision*vy_vision));
  
	float t = frames_ahead*tsample;
  
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
		state -> ball.velocity.x = vball_pred*(vx_vision)/vball_vision;
		state -> ball.velocity.y = vball_pred*(vy_vision)/vball_vision;
		state -> ball.Position.X = (xpos_vision + dist*(vx_vision)/vball_vision);
		state -> ball.Position.Y = (ypos_vision + dist*(vy_vision)/vball_vision);
	}

	state -> ball.velocity.x *= (float) 1000.0;
	state -> ball.velocity.y *= (float) 1000.0;
	state -> ball.Position.X *= (float) 1000.0;
	state -> ball.Position.Y *= (float) 1000.0;

	state -> ball.velocity.direction = atan((state -> ball.velocity.y)/(state -> ball.velocity.x));
	state -> ball.velocity.direction *= 180.0f / 3.1415f;
	if(state -> ball.velocity.x<0)
		state -> ball.velocity.direction+=180;
	while(state -> ball.velocity.direction>180)
		state -> ball.velocity.direction-=360;
	while(state -> ball.velocity.direction<-180)
		state -> ball.velocity.direction+=360;
	state -> ball.velocity.magnitude = sqrt ( ( state -> ball.velocity.x * state -> ball.velocity.x ) + ( state -> ball.velocity.y * state -> ball.velocity.y ) );
	
}*/

/*TVec2 ai09::calculateOpenAngleToGoal ( void )
{
	TVec2 ans = Vec2 ( 0 );

	ans.X = AngleWith ( Vec2 ( -side*3025,350 ) , ball.Position );
	ans.Y = AngleWith ( Vec2 ( -side*3025,-350 ) , ball.Position );

	if (0 )// OwnRobot[2].State.seenState != CompletelyOut )
	{
		float blockingRobotCenterAngle = AngleWith ( OwnRobot[2].State.Position , ball.Position );
		float blockingRobotMargin = asinDeg ( 90.0f / DIS ( ball.Position , OwnRobot[2].State.Position ) );

		float probableTargetAngle[2];
		probableTargetAngle[0] = ans.X - ( blockingRobotCenterAngle - blockingRobotMargin );
		probableTargetAngle[1] = ans.Y - ( blockingRobotCenterAngle + blockingRobotMargin );

		if ( fabs ( probableTargetAngle[0] ) > fabs ( probableTargetAngle[1] ) )
		{
			ans.Y = fabs ( probableTargetAngle[0] );
		}
		else
		{
			ans.X = blockingRobotCenterAngle + blockingRobotMargin;
			ans.Y = fabs ( probableTargetAngle[1] );
		}
	}
	else
	{
		ans.Y = fabs ( ans.X - ans.Y );
	}

	return ans;
}*/