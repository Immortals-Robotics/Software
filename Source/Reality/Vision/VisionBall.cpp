#include "Vision.h"
#include "../../Soccer/ai09/helpers.h"

#define MAX_BALL_2FRAMES_DISTANCE 1450000.0f

void VisionModule::ProcessBalls ( WorldState * state )
{
	int balls_num = 0;

	//First we have to extract the balls!
	balls_num = ExtractBalls ( );

	//Now lets merge them!
	balls_num = MergeBalls ( balls_num );

	//The most important part, The Kalman Filter!
	FilterBalls ( balls_num , state );

	//We're almost done, only Prediction remains undone!
	predictBallForward ( state );

}

int VisionModule::ExtractBalls ( void )
{
	int ans = 0;
	for ( int i = 0 ; i < Setting::kCamCount ; i ++ )
	{
		if (setting().use_camera[i] )
		{
			for ( int j = 0 ; j < frame[i].balls_size ( ) ; j ++ )
			{
				d_ball[ans] = frame[i].balls ( j );
//                t_capture_buff[ans] = frame[i].t_capture();

				ans ++;
			}
		}
	}
	return ans;
}
int VisionModule::MergeBalls ( int num )
{
	int balls_num = 0;
	for ( int i = 0 ; i < num ; i ++ )
	{
		for ( int j = i + 1 ; j < num ; j ++ )
		{
			if ( POWED_DIS ( d_ball[i].x() , d_ball[i].y() , d_ball[j].x() , d_ball[j].y() ) < MERGE_DISTANCE )
			{
				d_ball[i].set_x ( ( d_ball[i].x ( ) + d_ball[j].x ( ) ) / (float)2.0 );
				d_ball[i].set_y ( ( d_ball[i].y ( ) + d_ball[j].y ( ) ) / (float)2.0 );
				if ( d_ball[i].has_z ( ) )
					d_ball[i].set_z ( ( d_ball[i].z ( ) + d_ball[j].z ( ) ) / (float)2.0 );

//                t_capture_buff[i] = (t_capture_buff[i]+t_capture_buff[j]) / (float)2.0;

				d_ball[j] = d_ball[num-1];
//                t_capture_buff[j] = t_capture_buff[num-1];
				num --;

				j --;
			}
		}
		balls_num ++;
	}

	return balls_num;
}

void VisionModule::FilterBalls ( int num , WorldState * state )
{
	int id = 100;
	float dis = 214748364.0f;
	for ( int i = 0 ; i < num ; i ++ )
	{
		if ( POWED_DIS ( d_ball[i].x() , d_ball[i].y() , lastRawBall.x() , lastRawBall.y() ) < dis )
		{
			dis = POWED_DIS ( d_ball[i].x() , d_ball[i].y() , lastRawBall.x() , lastRawBall.y() );
			id = i;
		}
	}
//	std::cout << d_ball[id].x() <<std::endl
//		 << d_ball[id].y() <<std::endl;

	if ( dis < MAX_BALL_2FRAMES_DISTANCE )
	{
		float filtout[2][2];
		float filtpos[2] = { d_ball[id].x() / (float)10.0 , d_ball[id].y() / (float)10.0 };
		lastRawBall.CopyFrom ( d_ball[id] );

		if ( ball_not_seen > 0 )
		{
			ball_kalman.initializePos ( filtpos );
		}

		ball_kalman.updatePosition ( filtpos , filtout );

		state -> ball.Position.X = filtout[0][0] ;
		state -> ball.Position.Y = filtout[1][0] ;
		state -> ball.velocity.x = filtout[0][1] ;
		state -> ball.velocity.y = filtout[1][1] ;

		ball_not_seen = 0;
		state -> has_ball = true;
		state -> ball.seenState = Seen;
	}

	else
	{
		ball_not_seen ++;

		if ( ball_not_seen > MAX_BALL_NOT_SEEN )
		{
			if ( num > 0 )
			{
				float filtout[2][2];
				float filtpos[2] = { d_ball[id].x() / (float)10.0 , d_ball[id].y() / (float)10.0 };
				lastRawBall.CopyFrom ( d_ball[id] );
                ball_kalman.initializePos ( filtpos );
				
				ball_kalman.updatePosition ( filtpos , filtout );

				state -> ball.Position.X = filtout[0][0];
				state -> ball.Position.Y = filtout[1][0];
				state -> ball.velocity.x = filtout[0][1];
				state -> ball.velocity.y = filtout[1][1];


                ball_not_seen = 0;
				state -> has_ball = true;
				state -> ball.seenState = Seen;

            }
			else
			{
				state -> ball.velocity.x = 0;
				state -> ball.velocity.y = 0;

				state -> ball.Position.X /= (float)10.0;
				state -> ball.Position.Y /= (float)10.0;

				lastRawBall.set_x ( 0.0f );
				lastRawBall.set_y ( 0.0f );

				state -> has_ball = false;
				state -> ball.seenState = CompletelyOut;
			}
		}
		else
		{
			state -> ball.velocity.x /= (float)10.0;
			state -> ball.velocity.y /= (float)10.0;

			state -> ball.Position.X /= (float)10.0;
			state -> ball.Position.Y /= (float)10.0;

			state -> ball.seenState = TemprolilyOut;
		}
	}
	
}

void VisionModule::predictBallForward( WorldState * state )
{
	state -> ball.Position.X /= (float)100.0;
	state -> ball.Position.Y /= (float)100.0;
	state -> ball.velocity.x /= (float)100.0;
	state -> ball.velocity.y /= (float)100.0;
	float k = 0.25f; //velocity derate every sec(units (m/s)/s)
	float frame_rate = 61.0f;
	float tsample = (float)1.0f/(float)frame_rate;

	float vx_vision = state -> ball.velocity.x; 
	float vy_vision = state -> ball.velocity.y;

	float xpos_vision = state -> ball.Position.X;
	float ypos_vision = state -> ball.Position.Y;

	float vball_vision = float(sqrt(vx_vision*vx_vision + vy_vision*vy_vision));
  
	float t;
	if ( state -> ball.seenState == TemprolilyOut )
		t = tsample;
	else
		t = PREDICT_STEPS*tsample;

  
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


//    static int CNT = 0;
//    CNT++;
//    if(CNT%20 == 0) {
//        ball_dir_buff.push_back(state->ball.Position);
//        CNT = 0;
//    }
//
//    float tempAng,lastAng;
//    if(ball_dir_buff.size()) {
//        deque<TVec2>::iterator delete_until_here = ball_dir_buff.begin();
//        for (deque<TVec2>::iterator it = ball_dir_buff.begin(), it2 = ball_dir_buff.begin() + 1;
//             it2 != ball_dir_buff.end(); ++it, ++it2) {
//            tempAng = Angle(*it2 - *it);
//            if (it != ball_dir_buff.begin() && fabs(tempAng - lastAng) > 5) {
//                delete_until_here = it;
//            }
//            lastAng = tempAng;
//        }
//
//        while (delete_until_here != ball_dir_buff.begin())
//            ball_dir_buff.pop_front();
//		if(ball_dir_buff.size() >=2)
//        	state->ball.path_dir = ( ball_dir_buff.back() - ball_dir_buff.front());
//    }

//    float tempAngdelta = NormalizeAngle(Angle(state->ball.path_dir) - state -> ball.velocity.direction);
//    if(tempAngdelta != tempAngdelta)//tempAngdelta is NaN (Don't erase it)
//        tempAngdelta = 0.0;
//    if(fabs(tempAngdelta) > 15){
//        std::cout<<"NEW ANGLE____________"<<tempAngdelta<<std::endl;
//    }
//
//    if(state -> ball.velocity.magnitude > 10 && state -> ball.seenState == Seen && fabs(tempAngdelta) <= 15) {
//        state->ball.path_dir = (ball_dir_buff.front() - ball_dir_buff.back());
//        std::cout<<"we got here"<<std::endl;
//
//    }else if(state -> ball.seenState != TemprolilyOut){
//        state->ball.First_Pos_when_shooted = state->ball.Position;
//        std::cout<<"SEE: "<<fabs(tempAngdelta)<<std::endl;
//    }

}

void VisionModule::calculateBallHeight ( void )
{
	/*const float XO = -1358.2;
	const float YO = 60.93;
	float y0,x0,yp,xp,ya,xa,slopeAO,slopeL;
	y0 = ballBufferY[0];
	x0 = ballBufferX[0];*/

	//ballParabolic.calculate ( BALL_BUFFER_FRAMES , ballBufferY , ballBufferX );
	//std::cout << (int)ballParabolic.get_a() << std::endl;
}
