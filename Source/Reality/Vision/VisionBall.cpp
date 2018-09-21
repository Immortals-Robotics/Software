#include "Vision.h"

#include <limits>

#include <math/distance.h>

void Vision::ProcessBalls (WorldState & state)
{
	int balls_num = 0;

	//First we have to extract the balls!
	balls_num = ExtractBalls ( );

	//Now lets merge them!
	balls_num = MergeBalls ( balls_num );

	//The most important part, The Kalman Filter!
	FilterBalls ( balls_num , state );

	//We're almost done, only Prediction remains undone!
	PredictBall ( state );

}
int Vision::ExtractBalls ( void )
{
	int ans = 0;
	for ( int i = 0 ; i < config.camera_count() ; i ++ )
	{
		if ( config.camera_enabled(i) )
		{
			for ( int j = 0 ; j < frame[i].balls_size ( ) ; j ++ )
			{
				d_ball[ans] = frame[i].balls ( j );

				ans ++;

				if (ans >= d_ball.size())
				{
					break;
				}
			}
		}
	}
	return ans;
}
int Vision::MergeBalls ( int num )
{
	int balls_num = 0;
	for ( int i = 0 ; i < num ; i ++ )
	{
		for ( int j = i + 1 ; j < num ; j ++ )
		{
			if ( DIS ( d_ball[i].x() , d_ball[i].y() , d_ball[j].x() , d_ball[j].y() ) < config.merge_distance() )
			{
				d_ball[i].set_x ( ( d_ball[i].x ( ) + d_ball[j].x ( ) ) / (float)2.0 );
				d_ball[i].set_y ( ( d_ball[i].y ( ) + d_ball[j].y ( ) ) / (float)2.0 );
				if ( d_ball[i].has_z ( ) )
					d_ball[i].set_z ( ( d_ball[i].z ( ) + d_ball[j].z ( ) ) / (float)2.0 );

				d_ball[j] = d_ball[num-1];
				num --;

				j --;
			}
		}
		balls_num ++;
	}

	return balls_num;
}

void Vision::FilterBalls (int num, WorldState & state)
{
	int id = std::numeric_limits<int>::max();
	float dis = std::numeric_limits<float>::max();
	for ( int i = 0 ; i < num ; i ++ )
	{
		if ( DIS ( d_ball[i].x() , d_ball[i].y() , lastRawBall.x() , lastRawBall.y() ) < dis )
		{
			dis = DIS ( d_ball[i].x() , d_ball[i].y() , lastRawBall.x() , lastRawBall.y() );
			id = i;
		}
	}

	if ( dis < config.max_ball_2_frames_dis() )
	{
		float filtout[2][2];
		float filtpos[2] = { d_ball[id].x() / (float)10.0 , d_ball[id].y() / (float)10.0 };
		lastRawBall.CopyFrom ( d_ball[id] );

		if ( ball_not_seen > 0 )
		{
			ball_kalman.initializePos ( filtpos );
		}

		ball_kalman.updatePosition ( filtpos , filtout );

		state.ball.Position.X = filtout[0][0] ;
		state.ball.Position.Y = filtout[1][0] ;
		state.ball.velocity.x = filtout[0][1] ;
		state.ball.velocity.y = filtout[1][1] ;

		ball_not_seen = 0;
		state.has_ball = true;
		state.ball.seenState = Seen;
	}

	else
	{
		ball_not_seen ++;

		if ( ball_not_seen > config.max_ball_not_seen() )
		{
			if ( num > 0 )
			{
				float filtout[2][2];
				float filtpos[2] = { d_ball[id].x() / (float)10.0 , d_ball[id].y() / (float)10.0 };
				lastRawBall.CopyFrom ( d_ball[id] );
				ball_kalman.initializePos ( filtpos );
				
				ball_kalman.updatePosition ( filtpos , filtout );

				state.ball.Position.X = filtout[0][0];
				state.ball.Position.Y = filtout[1][0];
				state.ball.velocity.x = filtout[0][1];
				state.ball.velocity.y = filtout[1][1];

				ball_not_seen = 0;
				state.has_ball = true;
				state.ball.seenState = Seen;
			}
			else
			{
				state.ball.velocity.x = 0;
				state.ball.velocity.y = 0;

				state.ball.Position.X /= (float)10.0;
				state.ball.Position.Y /= (float)10.0;

				lastRawBall.set_x ( 0.0f );
				lastRawBall.set_y ( 0.0f );

				state.has_ball = false;
				state.ball.seenState = CompletelyOut;
			}
		}
		else
		{
			state.ball.velocity.x /= (float)10.0;
			state.ball.velocity.y /= (float)10.0;

			state.ball.Position.X /= (float)10.0;
			state.ball.Position.Y /= (float)10.0;

			state.ball.seenState = TemprolilyOut;
		}
	}
	
}

void Vision::PredictBall(WorldState & state)
{
	state.ball.Position.X /= (float)100.0;
	state.ball.Position.Y /= (float)100.0;
	state.ball.velocity.x /= (float)100.0;
	state.ball.velocity.y /= (float)100.0;
	float k = 0.25f; //velocity derate every sec(units (m/s)/s)
	float frame_rate = 61.0f;
	float tsample = (float)1.0f/(float)frame_rate;
  
	float vx_vision = state.ball.velocity.x; 
	float vy_vision = state.ball.velocity.y;
  
	float xpos_vision = state.ball.Position.X;
	float ypos_vision = state.ball.Position.Y;
  
	float vball_vision = Magnitude(vx_vision, vy_vision);
  
	float t;
	if ( state.ball.seenState == TemprolilyOut )
		t = tsample;
	else
		t = config.predict_steps()*tsample;

  
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
		state.ball.velocity.x = vball_pred*(vx_vision)/vball_vision;
		state.ball.velocity.y = vball_pred*(vy_vision)/vball_vision;
		state.ball.Position.X = (xpos_vision + dist*(vx_vision)/vball_vision);
		state.ball.Position.Y = (ypos_vision + dist*(vy_vision)/vball_vision);
	}

	state.ball.velocity.x *= (float) 1000.0;
	state.ball.velocity.y *= (float) 1000.0;
	state.ball.Position.X *= (float) 1000.0;
	state.ball.Position.Y *= (float) 1000.0;

	state.ball.velocity.direction = atan((state.ball.velocity.y)/(state.ball.velocity.x));
	state.ball.velocity.direction *= 180.0f / 3.1415f;
	if(state.ball.velocity.x<0)
		state.ball.velocity.direction+=180;
	while(state.ball.velocity.direction>180)
		state.ball.velocity.direction-=360;
	while(state.ball.velocity.direction<-180)
		state.ball.velocity.direction+=360;
	state.ball.velocity.magnitude = sqrt ( ( state.ball.velocity.x * state.ball.velocity.x ) + ( state.ball.velocity.y * state.ball.velocity.y ) );
	
}

void Vision::ComputeBallHeight ( void )
{
	/*const float XO = -1358.2;
	const float YO = 60.93;
	float y0,x0,yp,xp,ya,xa,slopeAO,slopeL;
	y0 = ballBufferY[0];
	x0 = ballBufferX[0];*/

	//ballParabolic.calculate ( BALL_BUFFER_FRAMES , ballBufferY , ballBufferX );
	//std::cout << (int)ballParabolic.get_a() << std::endl;
}
