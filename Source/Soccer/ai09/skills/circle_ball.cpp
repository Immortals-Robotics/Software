#include "ai09.h"

enum ball_circling_state {
	very_far = 0,
	far,
	near,
	kick
};

/*ball_circling_state circle_ball_very_far ( int robot_num , float tagret_angle , int chip_pow , int shoot_pow , float precision );
ball_circling_state circle_ball_far ( int robot_num , float tagret_angle , int chip_pow , int shoot_pow , float precision );
ball_circling_state circle_ball_near ( int robot_num , float tagret_angle , int chip_pow , int shoot_pow , float precision );
ball_circling_state circle_ball_behind ( int robot_num , float tagret_angle , int chip_pow , int shoot_pow , float precision );
ball_circling_state circle_ball_breath ( int robot_num , float tagret_angle , int chip_pow , int shoot_pow , float precision );
ball_circling_state circle_ball_kick ( int robot_num , float tagret_angle , int chip_pow , int shoot_pow , float precision );

ball_circling_state (*state_func[6])( int robot_num , float tagret_angle , int chip_pow , int shoot_pow , float precision ) = {
	&circle_ball_very_far,
	&circle_ball_far,
	&circle_ball_near,
	&circle_ball_behind,
	&circle_ball_breath,
	&circle_ball_kick};*/
	

void ai09::circle_ball ( int robot_num , float tagret_angle , int shoot_pow , int chip_pow , float precision , float near_dis_override )
{
	//tagret_angle -= 5;
	const float very_far_ball_dis = 600.0f;
	const float far_ball_dis = 160.0f;
	const int far_to_near_hys = 5;
    float near_ball_dis = 140.0f;
    if ( near_dis_override > 0 )
        near_ball_dis = near_dis_override;
	const float near_angle_tol = 4.0f;
	const int near_to_kick_hys = 3;
	const float shmit_coeff = 1.2f;
	
	static ball_circling_state state = very_far;
	static float last_change_t = 0.0f;
	static int hys_bank[4]={0,0,0,0};
	if (timer.time()<0.1) {
		state = very_far;
		last_change_t = timer.time();
		hys_bank[0]=hys_bank[1]=hys_bank[2]=hys_bank[3]=0;
		Halt(robot_num);
		return;
	}
	
	if (state == very_far) {
		OwnRobot[robot_num].face(ball.Position);
		ERRTSetObstacles(robot_num, 0, 1, 1, 1, 0, 1);
		ERRTNavigate2Point(robot_num, ball.Position, 1, 30, &VELOCITY_PROFILE_AROOM);
		
		AddDebugCircle(ball.Position,very_far_ball_dis-90.0f,Red);
		
		if (DIS(OwnRobot[robot_num].State.Position, ball.Position) < very_far_ball_dis) {
			state = far;
			last_change_t = timer.time();
		}
	}
	else if (state == far) {
		OwnRobot[robot_num].face(ball.Position);
		ERRTSetObstacles(robot_num, 0, 1, 1, 1, 0, 1);
		TVec2 target_point = CircleAroundPoint(ball.Position, AngleWith(ball.Position, OwnRobot[robot_num].State.Position), near_ball_dis);
		ERRTNavigate2Point(robot_num, target_point, 1, 20, &VELOCITY_PROFILE_AROOM);
		
		AddDebugCircle(ball.Position,far_ball_dis-90.0f,Pink);
		
		if (DIS(OwnRobot[robot_num].State.Position, ball.Position) < far_ball_dis) {
			hys_bank[0]++;
		}
		else {
			hys_bank[0]=0;
		}
		if (hys_bank[0] > far_to_near_hys ) {
			state = near;
			last_change_t = timer.time();
		}
		else if (DIS(OwnRobot[robot_num].State.Position, ball.Position) > very_far_ball_dis * shmit_coeff) {
			state = very_far;
			last_change_t = timer.time();
		}
	}
	else if (state == near) {
		float toRobot = AngleWith(ball.Position, OwnRobot[robot_num].State.Position);
		float newToRobot = NormalizeAngle(toRobot-tagret_angle);
		float deltaAngle = min(fabs(newToRobot), 30.0f);
		newToRobot = max(0.0f,fabs(newToRobot)-deltaAngle)*sgn(newToRobot);
		newToRobot = NormalizeAngle(newToRobot+tagret_angle);
		
		OwnRobot[robot_num].face(ball.Position);
		OwnRobot[robot_num].target.Angle += NormalizeAngle(newToRobot+180.0f-OwnRobot[robot_num].target.Angle)/2.0f;
		//OwnRobot[robot_num].target.Angle = NormalizeAngle(OwnRobot[robot_num].target.Angle);
		ERRTSetObstacles(robot_num, 0, 1, 1, 1, 0, 1);
		TVec2 target_point = CircleAroundPoint(ball.Position, newToRobot, near_ball_dis/cosDeg(deltaAngle));
        if ( near_dis_override > 0 )
            ERRTNavigate2Point(robot_num, target_point, 1, 20, &VELOCITY_PROFILE_AROOM);
        else
            ERRTNavigate2Point(robot_num, target_point, 1, 20, &VELOCITY_PROFILE_MAMOOLI);
		
		if (DIS(OwnRobot[robot_num].State.Position, ball.Position) > far_ball_dis*shmit_coeff) {
			state = far;
			last_change_t = timer.time();
		}
		
		if (fabs(deltaAngle) < near_angle_tol) {
			hys_bank[0]++;
		}
		else {
			hys_bank[0]=0;
		}
		
		if ((hys_bank[0]>near_to_kick_hys)&&((shoot_pow>0)||(chip_pow>0))) {
			state = kick;
			last_change_t = timer.time();
		}
	}
	
	else if (state == kick) {
		if (chip_pow>0) {
			chip_head = OwnRobot[robot_num].State.Angle;
		}
		//OwnRobot[robot_num].face(ball.Position);
		OwnRobot[robot_num].target.Angle=NormalizeAngle(tagret_angle+180.0f);
		ERRTSetObstacles(robot_num, 0, 1, 1, 1, 0, 1);
		ERRTNavigate2Point(robot_num, ball.Position, 1, 100, &VELOCITY_PROFILE_AROOM);
		if ( shoot_pow > 0 )
			OwnRobot[robot_num].Shoot(shoot_pow);
		if ( chip_pow > 0 )
			OwnRobot[robot_num].Chip(chip_pow);
		AddDebugCircle(ball.Position,very_far_ball_dis-90.0f,Red);
		//tech_circle(robot_num, tagret_angle, shoot_pow, chip_pow, 1, 1, 0, 0);
		
		if (DIS(OwnRobot[robot_num].State.Position, ball.Position) > near_ball_dis*shmit_coeff) {
			state = far;
			last_change_t = timer.time();
		}
	}

	
	AddDebugLine(OwnRobot[robot_num].State.Position,OwnRobot[robot_num].target.Position,Black);
	
}