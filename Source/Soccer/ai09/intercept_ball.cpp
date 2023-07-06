#include "ai09.h"

enum ball_intercept_state {
	kVeryFar = 0,
	kFar,
	kNear,
	kKick
};

void ai09::intercept_ball ( int robot_num , float angle , int shoot_pow , int chip_pow )
{
    static float last_change_t = 0.0f;
	static int hys_bank[4]={0,0,0,0};
    static ball_intercept_state state = kVeryFar;
    
    const float very_far_ball_dis = 600.0f;
	const float far_ball_dis = 150.0f;
	const int far_to_near_hys = 5;
    float near_ball_dis = 140.0f;
    const float near_angle_tol = 4.0f;
	const int near_to_kick_hys = 3;
	const float shmit_coeff = 1.2f;
    
    if (state == kVeryFar) {
        
        float ball_intercept_t = calculateBallRobotReachTime(robot_num, &VELOCITY_PROFILE_MAMOOLI);
        TVec2 ball_reach_point = predictBallForwardAI(ball_intercept_t);
        
        if (DIS(OwnRobot[robot_num].State.Position, ball.Position) < very_far_ball_dis) {
			state = kFar;
			last_change_t = timer.time();
		}
    }
    
    else if (state == kFar) {
		
        
        if (DIS(OwnRobot[robot_num].State.Position, ball.Position) < far_ball_dis) {
			hys_bank[0]++;
		}
		else {
			hys_bank[0]=0;
		}
		if (hys_bank[0] > far_to_near_hys ) {
			state = kNear;
			last_change_t = timer.time();
		}
		else if (DIS(OwnRobot[robot_num].State.Position, ball.Position) > very_far_ball_dis * shmit_coeff) {
			state = kVeryFar;
			last_change_t = timer.time();
		}
	}
    
    else if (state == kNear) {
		
		
		if (DIS(OwnRobot[robot_num].State.Position, ball.Position) > far_ball_dis*shmit_coeff) {
			state = kFar;
			last_change_t = timer.time();
		}
		
		//if (fabs(deltaAngle) < near_angle_tol) {
			hys_bank[0]++;
		//}
		//else {
		//	hys_bank[0]=0;
		//}
		
		if ((hys_bank[0]>near_to_kick_hys)&&((shoot_pow>0)||(chip_pow>0))) {
			state = kKick;
			last_change_t = timer.time();
		}
	}
	
	else if (state == kKick) {
		
		if (DIS(OwnRobot[robot_num].State.Position, ball.Position) > near_ball_dis*shmit_coeff) {
			state = kFar;
			last_change_t = timer.time();
		}
	}
}