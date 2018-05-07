#include "Robot.h"
#include <iostream>
using namespace std;
float getCalibratedShootPow ( int vision_id , float raw_shoot )
{
	if (raw_shoot<=0) {
		return 0;
	}
	vision_id = min(11, max(0,vision_id));

	static float poly_coeff[MAX_ROBOTS][3] = {
		{-0.1012,6.8441,-27.822}, //  0
		{-0.0988,6.5467,-28.173}, //  1
		{-0.1012,6.8645,-28.968}, //  2
		{-0.09,6.2632,-22.909}, //  3
		{-0.0789,4.8728,-18.189}, //  4
		{-0.0859,6.2805,-27.016}, //  5
		{-0.0857,6.2963,-25.36}, //  6
		{-0.0813,6.1029,-21.593}, //  7
		{-0.0813,6.1029,-21.593}, //  8*
		{-0.0813,6.1029,-21.593}, //  9*
		{-0.0813,6.1029,-21.593}, // 10*
		{-0.0813,6.1029,-21.593}, // 11*
	};




	float a = poly_coeff[vision_id][0];
	float b = poly_coeff[vision_id][1];
	float c = poly_coeff[vision_id][2];

	raw_shoot = min(150.0f, max(0.0f,raw_shoot));
	
	float delta = b*b-4.0f*a*(c-raw_shoot);
	if (delta<0)
		delta = 0;
	
	float calib_shoot = (-b+sqrt(delta))/(2.0f*a);
	
	calib_shoot = min(100.0f, max(0.0f,calib_shoot));
			
	return calib_shoot;

}

float getCalibratedChipPow ( int vision_id , float dis_raw )
{
	if (dis_raw<=0) {
		return 0;
	}
	vision_id = min(11, max(0,vision_id));
	
	static float poly_coeff[MAX_ROBOTS][3] = {
		{-0.0586,7.6894,-121.18}, //  0
		{-0.0845,12.051,-197.31}, //  1
		{-0.0533,9.0368,-164.51}, //  2
		{-0.0401,8.632,-139.31}, //  3*
		{-0.08,10.804,-166.69}, //  4
		{-0.0533,9.0368,-164.51}, //  5
		{-0.0953,12.555,-195.92}, //  6
		{-0.0291,7.0266,-103.61}, //  7
		{-0.0533,9.0368,-164.51}, //  8*
		{-0.0533,9.0368,-164.51}, //  9*
		{-0.0263,5.3907,-84.211}, //  10*
		{-0.0926,12.61,-203.72}, //  11*
	};
    
    static float ghuz_coeffs[MAX_ROBOTS] = {
        220.0,  //  0
        260.0,  //  1
        180.0,  //  2
        250.0,  //  3
        210.0,  //  4
        200.0,  //  5
        150.0,  //  6
        200.0,  //  7
        265.0,  //  8
        265.0,  //  9
        140.0,  //  A
        180.0  //  B
        
    };
	
	float a = poly_coeff[vision_id][0];
	float b = poly_coeff[vision_id][1];
	float c = poly_coeff[vision_id][2];
	
	dis_raw = min(500.0f, max(0.0f,dis_raw));
	
	float delta = b*b-4.0f*a*(c-dis_raw);
	if (delta<0)
		delta = 0;
	
	float calib_shoot = (-b+sqrt(delta))/(2.0f*a);
	//calib_shoot *= 1.25f;
    
    calib_shoot *= pow(ghuz_coeffs[1] / ghuz_coeffs[vision_id],0.5);
	
	calib_shoot = min(100.0f, max(0.0f,calib_shoot));
	
	return calib_shoot;

}


	Robot::Robot(void){
		oldRobot = false;
		
		CMDindex = 0;
		for ( int i = 0 ; i < 10 ; i ++ )
			lastCMDs[i] = Vec3 ( 0.0f );
		
		trapezoid.init ( Vec2 ( 4500.0f ) , Vec2 ( 1400.0f ) , Vec2 ( 3000.0f ) , 1.0f/61.0f );
		
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
		chip = getCalibratedShootPow(vision_id, pow);
		//chip = pow;
	}
	void Robot::Chip(int pow){
		shoot = getCalibratedChipPow(vision_id, pow);
		//shoot = pow;
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
		if ( fabs ( target.Position.X ) > field_w + 150.0 )
			target.Position.X = sgn ( target.Position.X ) * (field_w + 150.0);

		if ( fabs ( target.Position.Y ) > field_h + 150.0 )
			target.Position.Y = sgn ( target.Position.Y ) * (field_h + 150.0);
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


		motion.X *= 2.55;
		motion.Y *= 2.55;
		//motion.Z /= 3.0;

		int VelX = motion.X;
		int VelY = motion.Y;
		int targetAng = target.Angle;

		data[3] = abs(VelX);//VelX
		data[4] = abs(VelY);//VelY

		data[6] = abs(targetAng);


		data[7] = 0x00;//the signes

		if (target.Angle < 0)
			data[7] |= 0x80;
		if ( motion.Y < 0 )
			data[7] |= 0x20;
		if ( motion.X < 0 )
			data[7] |= 0x10;



	}
	
	void Robot::makeSendingDataReady ( void )
	{

        if(halted) {
            data[1] = 0x0A;//length=10
            data[2] = 0x06;//Command to HALT
            data[3] = 0x00;
            data[4] = 0x00;
            data[5] = 0x00;
            data[6] = 0x00;
            data[7] = 0x00;
            data[8] = 0x00;
            data[9] = 0x00;
        }
        else {
            int currAng = State.Angle;

            data[0] = this->vision_id;//Robots ID
            data[1] = 0x0A;//length=10
            data[2] = 0x04;//Command to move by Vel

            data[5] = abs(currAng);//Current angle

            if (State.Angle < 0)
                data[7] |= 0x40;

            if (shoot > 0) {
                data[8] = shoot;
                data[9] = 0x00;
            } else if (chip > 0) {
                data[8] = 0x00;
                data[9] = chip;
            } else {
                data[8] = 0x00;
                data[9] = 0x00;
            }



            //data[0]=0x07;
            //data[1]=0x0A;
            //data[2]=0x04;
//            data[3] = 0x10;
//            data[4] = 0x00;
//            data[5] = 0x00;
//            data[6] = 0x00;
//            data[7] = 0x00;
//            data[8] = 0x00;
//            data[9] = 0x00;
        }
		dribbler = 0;
		shoot = 0;
		chip = 0;
		Break = 0;

	}