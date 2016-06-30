#include "helpers.h"
#include "trapezoid.h"
#include "VelocityProfile.h"
#include <math.h>
#include <iostream>
#include "../../Common/timer.h"

#include "../../Reality/Com/data_lite.h"

#define PREDICT_CMDS 5

class Robot
{
	Timer angleSendTimer;

	public :

	robot_command_msg_t command_msg;

    float field_w;
    float field_h;
    
	RobotState State;
	RobotState target;
	int shoot , chip , Break , dribbler;
	int Motor[4];
	
	int vision_id;
	bool halted;
	
	TVec3 lastCMDs[11];
	int CMDindex;
	
	Robot(void);
	
	void set_vision_id(unsigned short v_id);
	
	void set_control_mode(bool c_mode);
	
	float dis(float x1,float y1,float x2,float y2);
	
	void Shoot(int pow);
	
	void Chip(int pow);
	
	void Dribble(int pow);
	
	void face(TVec2 _target);
	
	TVec3 MotionPlan ( RobotState state , RobotState target , float speed , bool accurate , TVec3 * cmd , VelocityProfile * velocityProfile );
	
	void Move(bool accurate , float speed , VelocityProfile * velocityProfile );
	
	void MoveByMotion(TVec3 motion);
	
	void makeSendingDataReady ( void );
	
};