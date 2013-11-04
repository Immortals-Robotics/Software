#pragma once

#include "Vector.h"
#include "WorldState.h"
#include "helpers.h"
#include "trapezoid.h"
#include <math.h>
#include <iostream>

class Robot
{
	public :
	bool oldRobot;
	RobotState State;
	RobotState target;
	int shoot , chip , Break , dribbler;
	int Motor[4];
	unsigned char data[10];
	int serial_id,vision_id;
	bool control_mode;
	
	TVec3 lastCMDs[10];
	int CMDindex;
	
	TrapezoidPlanner trapezoid;
	
	int remainingPIDParams;
	float p , i , iMax , torque;
	
	Robot(void);
		
	void sendPID ( float _p , float _i , float _iMax , float _torque );
	
	void set_serial_id(unsigned short s_id);
	
	void set_vision_id(unsigned short v_id);
	
	void set_control_mode(bool c_mode);
	
	void Shoot(int pow);
	
	void Chip(int pow);
	
	void Dribble(int pow);
	
	void face(TVec2 _target);

	TVec3 MotionPlan ( RobotState state , RobotState target , float speed  , bool accurate , TVec3 * cmd );
	
	void Move(bool accurate , float speed );

	void MoveByMotion(TVec3 motion);
	
	void makeSendingDataReady ( void );
};