#include "helpers.h"
#include "trapezoid.h"
#include "VelocityProfile.h"
#include <math.h>
#include <iostream>
#include "../../Common/timer.h"

#define PREDICT_CMDS 7

class Robot
{
	Timer angleSendTimer;

	public :

    float field_w;
    float field_h;
    
	bool oldRobot;
	RobotState State;
	RobotState target;
	int shoot , chip , Break , dribbler;
	int Motor[4];
	unsigned char data[11];
	int serial_id,vision_id;
	bool control_mode;
	bool halted;
	
	TVec3 lastCMDs[11];
	int CMDindex;
	
	TrapezoidPlanner trapezoid;
	
	int remainingPIDParams;
	float p , i , iMax , torque;
	
	Robot(void);
	
	void sendPID ( float _p , float _i , float _iMax , float _torque );
	
	void set_serial_id(unsigned short s_id);
	
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