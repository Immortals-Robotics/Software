#include "Vector.h"
#include "WorldState.h"
#include "helpers.h"
#include "trapezoid.h"
#include "VelocityProfile.h"
#include <math.h>
#include <iostream>

#define PREDICT_CMDS 7

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
	bool halted;
	
	TVec3 lastCMDs[11];
	int CMDindex;
	
	TrapezoidPlanner trapezoid;
	
	int remainingPIDParams;
	float p , i , iMax , torque;
	
	Robot(void){
		oldRobot = false;
		
		CMDindex = 0;
		for ( int i = 0 ; i < 10 ; i ++ )
			lastCMDs[i] = Vec3 ( 0.0f );
		
		trapezoid.init ( Vec2 ( 4500.0f ) , Vec2 ( 4000.0f ) , Vec2 ( 3000.0f ) , 1.0f/61.0f );
		
		State.velocity.x = 0.0f;
		State.velocity.y = 0.0f;
		State.Position = Vec2 ( 0.0f );
		State.velocity.magnitude = 0.0f;
		State.velocity.direction = 0.0f;
		shoot = 0;
		chip = 0;
		dribbler = 0;
		Break = false;
		halted = false;
		data[0] = 1;
		data[9] = 200;
	}
	
	void sendPID ( float _p , float _i , float _iMax , float _torque )
	{
		p = ( _p - 5 ) * 10;
		i = _i * 500;
		iMax = _iMax / 4;
		torque = ( _torque - 1000 ) / 4;
		remainingPIDParams = 15;
	}
	
	void set_serial_id(unsigned short s_id){
		serial_id = s_id;
	}
	
	void set_vision_id(unsigned short v_id){
		vision_id = v_id;
	}
	
	void set_control_mode(bool c_mode)
	{
		control_mode=c_mode;
	}
	
	float dis(float x1,float y1,float x2,float y2){
		return sqrt(pow(x2-x1,2)+pow(y2-y1,2));
	}
	
	void Shoot(int pow){
		chip = pow;
	}
	void Chip(int pow){
		shoot = pow;
	}
	
	void Dribble(int pow){
		dribbler = 16*pow;
	}
	
	
	
	void face(TVec2 _target){
		target.Angle = AngleWith(State.Position, _target);
	}
	
	TVec3 MotionPlan ( RobotState state , RobotState target , float speed , bool accurate , TVec3 * cmd , VelocityProfile * velocityProfile );
	
	void Move(bool accurate , float speed , VelocityProfile * velocityProfile ){
		//if ( State.vision_id == 4 )
		//else
		if ( fabs ( target.Position.X ) > 3500 )
			target.Position.X = sgn ( target.Position.X ) * 3500.0f;

		if ( fabs ( target.Position.Y ) > 2200 )
			target.Position.Y = sgn ( target.Position.Y ) * 2200.0f;
		target.Angle = NormalizeAngle ( target.Angle );
		if ( speed < 0 )
			speed = 0;
		if ( speed > 100 )
			speed = 100;
		MoveByMotion ( MotionPlan ( State , target , speed , accurate , lastCMDs, velocityProfile ) );
		//MoveByMotion ( trapezoid.Plan ( &State , &target ) );
	}
	
	void MoveByMotion(TVec3 motion)
	{
		//motion.X=0;
		lastCMDs[CMDindex] = motion;
		lastCMDs[10].X = CMDindex;
		lastCMDs[10].Y = PREDICT_CMDS;

		CMDindex ++;
		if ( CMDindex > PREDICT_CMDS - 1 )
			CMDindex = 0;
				
		//std::cout << serial_id << "	:	" << CMDindex << std::endl;
		
		float trans_rad = ( 90.0f - State.Angle ) * ( 3.1415f / 180.0f );
		motion = Vec3 ( 
					   cos(trans_rad)*motion.X - sin(trans_rad)*motion.Y , 
					   sin(trans_rad)*motion.X + cos(trans_rad)*motion.Y ,
					   motion.Z );
		
		if ( oldRobot )
		{
			Motor[1]=(int)(127.0+motion.Y*0.8-motion.X*0.6-motion.Z/12.0);
			Motor[0]=(int)(127.0-motion.Y*0.8-motion.X*0.6-motion.Z/12.0);
			Motor[3]=(int)(127.0-motion.Y*0.707+motion.X*0.707-motion.Z/12.0);
			Motor[2]=(int)(127.0+motion.Y*0.707+motion.X*0.707-motion.Z/12.0);
			
			for(int i=0;i<4;i++)
			{
				data[i+2] = Motor[i];
				if ( data[i+2] == 0 )
					data[i+2] ++;
			}
		}
		
		else
		{
			motion.X *= 2.55;
			motion.Y *= 2.55;
			//motion.Z /= 3.0;

			/*motion.Z *= 2.0;
			 motion.Z /= 1.8;*/
			
			//motion = Vec3 ( 0 , 0 , -100 );
			
			/*motion.X = -10;
			motion.Y = -10;
			motion.Z = 100;*/
			
			data[1] = serial_id + ( sgnBool ( motion.Z ) * 16 ) + ( sgnBool ( motion.X ) * 32 ) + ( sgnBool ( motion.Y ) * 64 ) ;
			data[2] = fabs ( motion.Z );
			data[3] = fabs ( motion.X );
			data[4] = fabs ( motion.Y );

			
			
		}
	}
	
	void makeSendingDataReady ( void )
	{
		if ( oldRobot )
		{
			data[1] = serial_id + ( chip * 2 );
			
			data[6] = 4 + shoot * 2;
			data[7] = 2;
			data[8] = 0;
			data[9] = 0;
		}
		
		else
		{
			//shoot = chip = 0;
			data[5] = shoot;
			data[6] = chip;
			if ( shoot > 0 || chip > 0 )
				data[7] = 24;	//8:gyro, 16:force, 24:god help us all!
			else
				data[7] = 1;
			if (dribbler>0)
			{
				data[7]+=64;
			}
			if (halted) {
				data[7] = 6;
			}
			
			//data[8] = dribbler + 1;
			
			int checkSum = 0;
			for ( int i = 0 ; i < 8 ; i ++ )
			{
				checkSum += (data[i]);
			}
			//data[9] = checkSum % 255;
			//data[9] += 1;
						
			/*if ( remainingPIDParams > 0 )
			{
				remainingPIDParams --;
				data[7] = 6;
				data[5] = 115;//p + 1;
				data[6] = 180;//i + 1; 
				data[8] = 250;//iMax + 1;
				data[9] = torque + 1;
			}*/
		}
		
		dribbler = 0;
		shoot = 0;
		chip = 0;
		Break = 0;
	}
};