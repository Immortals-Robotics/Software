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

	std::vector<int> ball_ids;
	ball_id_generator->Update(d_ball, balls_num, frame[0].t_capture(), ball_ids);

	//The most important part, The Kalman Filter!
	FilterBalls ( balls_num , ball_ids);

	//We're almost done, only Prediction remains undone!
	PredictBall ();

	FillBallStates(state);

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

void Vision::FilterBalls (int num, const std::vector<int>& ball_ids)
{
	for (int i = 0; i < config.max_balls(); i++)
	{
		bool found = false;
		float filt_out[2][2];
		for (int j = 0; j < num; j++)
		{
			if (ball_ids[j] == i)
			{
				found = true;
				
				float filt_pos[2] = { d_ball[j].x() / 10.0f , d_ball[j].y() / 10.0f };

				if (ball_not_seen[i] > 0)
				{
					ball_kalman[i].initializePos(filt_pos);
				}

				ball_not_seen[i] = 0;

				ball_kalman[i].updatePosition(filt_pos, filt_out);
			}
		}

		if (!found)
		{
			ball_not_seen[i] ++;
			if (ball_not_seen[i] >= config.max_ball_not_seen() + 1)
				ball_not_seen[i] = config.max_ball_not_seen() + 1;

			ballState[i].velocity.x -= ballState[i].velocity.x * 0.1f;
			ballState[i].velocity.y -= ballState[i].velocity.y * 0.1f;
		}
		else
		{
			ballState[i].Position.X = filt_out[0][0];
			ballState[i].Position.Y = filt_out[1][0];
			ballState[i].velocity.x = filt_out[0][1];
			ballState[i].velocity.y = filt_out[1][1];

			// Make sure our filtered velocities are reasonable

			if (Magnitude(ballState[i].velocity.x, ballState[i].velocity.y) >
				config.robot_error_velocity())
			{
				ballState[i].velocity.x = 0.0f;
				ballState[i].velocity.y = 0.0f;
			}

			if (fabs(ballState[i].velocity.x) < config.ignore_prediction() * 2.0f)
				ballState[i].velocity.x = 0.0f;
			if (fabs(ballState[i].velocity.y) < config.ignore_prediction() * 2.0f)
				ballState[i].velocity.y = 0.0f;

			ballState[i].Position.X *= (float)(10.0);
			ballState[i].Position.Y *= (float)(10.0);
			ballState[i].velocity.x *= (float)(10.0);
			ballState[i].velocity.y *= (float)(10.0);
		}

		ballState[i].velocity.direction = atan((ballState[i].velocity.y) / (ballState[i].velocity.x));
		ballState[i].velocity.direction *= 180.0f / 3.1415f;
		if (ballState[i].velocity.x < 0)
			ballState[i].velocity.direction += 180;
		while (ballState[i].velocity.direction > 180)
			ballState[i].velocity.direction -= 360;
		while (ballState[i].velocity.direction < -180)
			ballState[i].velocity.direction += 360;

		ballState[i].velocity.magnitude = sqrt((ballState[i].velocity.x * ballState[i].velocity.x) + (ballState[i].velocity.y * ballState[i].velocity.y));
	}
}

void Vision::PredictBall()
{
	for (int i = 0; i < config.max_balls(); i++)
	{
		ballState[i].Position.X /= 1000.0f;
		ballState[i].Position.Y /= 1000.0f;
		ballState[i].velocity.x /= 1000.0f;
		ballState[i].velocity.y /= 1000.0f;
		float k = 0.25f; //velocity derate every sec(units (m/s)/s)
		float frame_rate = 61.0f;
		float tsample = (float)1.0f / (float)frame_rate;

		float vx_vision = ballState[i].velocity.x;
		float vy_vision = ballState[i].velocity.y;

		float xpos_vision = ballState[i].Position.X;
		float ypos_vision = ballState[i].Position.Y;

		float vball_vision = Magnitude(vx_vision, vy_vision);

		float t;
		if (ballState[i].seenState == TemprolilyOut)
			t = tsample;
		else
			t = 0;//config.predict_steps()*tsample;


		float v = vball_vision - k * t;
		float dist0 = vball_vision * t - k * (t*t) / 2.0f;

		float dist;
		float vball_pred;

		// if speed turns out to be negative..it means that ball has stopped, so calculate that amount of 
		// distance traveled
		if (v < 0) {
			vball_pred = 0.0f;
			dist = (vball_vision*vball_vision)*k / 2.0f;
			// i.e the ball has stopped, so take a newer vision data for the prediction
		}
		else {
			vball_pred = v;
			dist = dist0;
		}

		if (vball_vision != 0) {
			ballState[i].velocity.x = vball_pred * (vx_vision) / vball_vision;
			ballState[i].velocity.y = vball_pred * (vy_vision) / vball_vision;
			ballState[i].Position.X = (xpos_vision + dist * (vx_vision) / vball_vision);
			ballState[i].Position.Y = (ypos_vision + dist * (vy_vision) / vball_vision);
		}

		ballState[i].velocity.x *= (float) 1000.0;
		ballState[i].velocity.y *= (float) 1000.0;
		ballState[i].Position.X *= (float) 1000.0;
		ballState[i].Position.Y *= (float) 1000.0;

		ballState[i].velocity.direction = atan((ballState[i].velocity.y) / (ballState[i].velocity.x));
		ballState[i].velocity.direction *= 180.0f / 3.1415f;
		if (ballState[i].velocity.x < 0)
			ballState[i].velocity.direction += 180;
		while (ballState[i].velocity.direction > 180)
			ballState[i].velocity.direction -= 360;
		while (ballState[i].velocity.direction < -180)
			ballState[i].velocity.direction += 360;
		ballState[i].velocity.magnitude = sqrt((ballState[i].velocity.x * ballState[i].velocity.x) + (ballState[i].velocity.y * ballState[i].velocity.y));
	}
}

void Vision::FillBallStates(WorldState& state)
{
	state.has_ball = false;
	state.balls_num = 0;
	for (int i = 0; i < config.max_balls(); i++)
	{
		ballState[i].id = i;

		state.balls_num++;
		if (ball_not_seen[i] == 0)
		{
			ballState[i].seenState = Seen;
		}
		else if (ball_not_seen[i] < config.max_ball_not_seen())
		{
			ballState[i].seenState = TemprolilyOut;
		}
		else
		{
			ballState[i].seenState = CompletelyOut;
			state.balls_num--;
		}

		if (ballState[i].seenState != CompletelyOut && !state.has_ball)
		{
			state.ball = ballState[i];
			state.has_ball = true;
		}

		state.balls[i] = ballState[i];
	}

	printf("balls: %d\n", state.balls_num);
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
