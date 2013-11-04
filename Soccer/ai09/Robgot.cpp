#include "Robot.h"

	Robot::Robot(void){
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
		
		angleSendTimer.start();
	}
	
	void Robot::sendPID ( float _p , float _i , float _iMax , float _torque )
	{
		p = ( _p - 5 ) * 10;
		i = _i * 500;
		iMax = _iMax / 4;
		torque = ( _torque - 1000 ) / 4;
		remainingPIDParams = 15;
	}
	
	void Robot::set_serial_id(unsigned short s_id){
		serial_id = s_id;
	}
	
	void Robot::set_vision_id(unsigned short v_id){
		vision_id = v_id;
	}
	
	void Robot::set_control_mode(bool c_mode)
	{
		control_mode=c_mode;
	}
	
	float Robot::dis(float x1,float y1,float x2,float y2){
		return sqrt(pow(x2-x1,2)+pow(y2-y1,2));
	}
	
	void Robot::Shoot(int pow){
		chip = pow;
	}
	void Robot::Chip(int pow){
		shoot = pow;
	}
	
	void Robot::Dribble(int pow){
		dribbler = 16*pow;
	}
	
	
	
	void Robot::face(TVec2 _target){
		target.Angle = AngleWith(State.Position, _target);
	}
		
	void Robot::Move(bool accurate , float speed , VelocityProfile * velocityProfile ){
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
	
	void Robot::MoveByMotion(TVec3 motion)
	{
		//motion.X=0;
		lastCMDs[CMDindex] = motion;
		lastCMDs[10].X = CMDindex;
		lastCMDs[10].Y = PREDICT_CMDS;

		CMDindex ++;
		if ( CMDindex > PREDICT_CMDS - 1 )
			CMDindex = 0;
				
		//std::cout << serial_id << "	:	" << CMDindex << std::endl;
		
		/*float trans_rad = ( 90.0f - State.Angle ) * ( 3.1415f / 180.0f );
		motion = Vec3 ( 
					   cos(trans_rad)*motion.X - sin(trans_rad)*motion.Y , 
					   sin(trans_rad)*motion.X + cos(trans_rad)*motion.Y ,
					   motion.Z );*/
		
		
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

			data[2] = fabs ( motion.X );
			data[3] = fabs ( motion.Y );
			data[5] = fabs ( target.Angle );
			
			data[10] &= 0x0F;
			
			if ( target.Angle < 0 )
				data[10] |= 128;
			if ( motion.Y < 0 )
				data[10] |= 32;
			if ( motion.X < 0 )
				data[10] |= 16;
			
			data[6] = 2;
			
		}
	}
	
	void Robot::makeSendingDataReady ( void )
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
			data[0] = serial_id;
			data[1] = 1; //Set the packet type to "command".
			
			if ((State.seenState==Seen)&&(halted))//&&( angleSendTimer.time() > 1.0 ))// ((halted)&&(State.seenState==Seen)) ||( angleSendTimer.time() > 1.0 ) )
			{
				data[4] = fabs ( State.Angle );
				if ( State.Angle < 0 )
					data[10] |= 64;
				angleSendTimer.start();

			}
			else {
				data[4] = 200;
			}
			
			//shoot = chip = 0;
			data[7] = shoot;
			data[8] = chip;
			data[9] = 0;
			
			data[10] &= 0xF0;
			
			if (dribbler>0)
			{
				data[10]|= 2;
			}
			if (!halted) {
				data[10] |= 1;
			}
		}
		
		dribbler = 0;
		shoot = 0;
		chip = 0;
		Break = 0;
	}