#include "Robot.h"
#include <iostream>

static float shoot_coeffs[Setting::kMaxRobots][2] = {
		{6.68088f,  4.88984f}, //  0
		{5.47922f,  7.44389f}, //  1
		{8.42397f, 11.95175f}, //  2
		{7.81809f,  3.22943f}, //  3
		{0,         0       }, //  4
		{4.05175f,  9.08842f}, //  5
		{6.81159f,  12.43478}, //  6
		{0,        0        }, //  7
		{0,        0        }, //  8*
		{0,        0        }, //  9*
		{0,        0        }, // 10*
		{0,        0        }, // 11*
};

static float chip_coeffs[Setting::kMaxRobots][2] = {
		{5.41180f, 12.58416f}, //  0
		{4.19252f, 15.25629f}, //  1
		{9.14946f,  8.34962f}, //  2
		{4.90275f, 16.94857f}, //  3
		{0,         0       }, //  4
		{5.48581f,  9.45330f}, //  5
		{7.88439f,  6.42894f}, //  6
		{0,        0        }, //  7
		{0,        0        }, //  8*
		{0,        0        }, //  9*
		{0,        0        }, // 10*
		{0,        0        }, // 11*
};

float getCalibratedShootPow ( int vision_id , float raw_shoot, float coeffs[Setting::kMaxRobots][2])
{
	if (raw_shoot<=0) {
		return 0;
	}
	vision_id = min(Setting::kMaxRobots, max(0,vision_id));

	const float a = coeffs[vision_id][0];
	const float b = coeffs[vision_id][1];

	raw_shoot = min(150.0f, max(0.0f,raw_shoot));
	
	float calib_shoot = a * raw_shoot + b;
	calib_shoot = min(100.0f, max(0.0f,calib_shoot));
			
	return calib_shoot;
}

Robot::Robot(void){
	oldRobot = false;
	new_comm_ready = false;

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
	switch (vision_id)
	{
		case 2:
		case 4:
		case 6:
			shootMult = 1.3f;
			break;
		default:
			shootMult = 0.9f;
			break;
	}
}

void Robot::set_control_mode(bool c_mode)
{
	control_mode=c_mode;
}

float Robot::dis(float x1,float y1,float x2,float y2){
	return sqrt(pow(x2-x1,2)+pow(y2-y1,2));
}

void Robot::Shoot(int pow){
    shoot = getCalibratedShootPow(vision_id, pow / 7.5f, shoot_coeffs);
}
void Robot::Chip(int pow){
	chip = getCalibratedShootPow(vision_id, pow / 2.0f, chip_coeffs);
}

void Robot::Dribble(int pow){
	dribbler = 16*pow;
}



void Robot::face(TVec2 _target){
	target.Angle = AngleWith(State.Position, _target);
}

TVec3 Robot::ComputeMotionCommand(bool accurate, float speed, VelocityProfile *velocityProfile)
{
	const float field_extra_area = 200.f;

	if ( fabs ( target.Position.X ) > field_w + field_extra_area )
		target.Position.X = sgn ( target.Position.X ) * (field_w + field_extra_area);

	if ( fabs ( target.Position.Y ) > field_h + field_extra_area )
		target.Position.Y = sgn ( target.Position.Y ) * (field_h + field_extra_area);
	target.Angle = NormalizeAngle ( target.Angle );

	if ( speed < 0 )
		speed = 0;
	if ( speed > 100 )
		speed = 100;

	TVec3 motion = MotionPlan ( State , target , speed , accurate , lastCMDs, velocityProfile );

	target.velocity.x = 0;
	target.velocity.y = 0;

	return motion;
}

void Robot::MoveByMotion(TVec3 motion)
{
	motion.X = min(100, max(-100, motion.X));
	motion.Y = min(100, max(-100, motion.Y));
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
    if(true) {//new protocol

//        velocity.x.f32 = motion.X;
//        velocity.y.f32 = motion.Y;
//        target_orientation.f32 = target.Angle;
        convert_float_to_2x_buff(data + 3,motion.X);
        convert_float_to_2x_buff(data + 5,motion.Y);
        convert_float_to_2x_buff(data + 7,target.Angle);
    }else {


        int VelX = motion.X;
        int VelY = motion.Y;
        int targetAng = target.Angle;

        data[3] = abs(VelX);//VelX
        data[4] = abs(VelY);//VelY

        if (vision_id == 7) {
			std::cout << "speed in X axis: " << (int) data[3] << '-' << motion.X << std::endl;
			std::cout << "speed in Y axis: " << (int) data[4] << '-' << motion.X << std::endl;
        }

        data[6] = abs(targetAng);


        data[7] = 0x00;//the signes

        if (target.Angle < 0)
            data[7] |= 0x80;
        if (motion.Y < 0)
            data[7] |= 0x20;
        if (motion.X < 0)
            data[7] |= 0x10;
    }


}

void Robot::makeSendingDataReady ( void )
{


	data[0] = this->vision_id;
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
        if (true){//new protocol
            data[1] = 15;//length=10
            data[2] = 12;//Command to move with new protocol

            convert_float_to_2x_buff(data + 9,State.Angle);
            if (shoot > 0) {
                data[11] = shoot;
                data[12] = 0x00;
            } else if (chip > 0) {
                data[11] = 0x00;
                data[12] = chip;
            } else {
                data[11] = 0x00;
                data[12] = 0x00;
            }
//            data[11] = 0x00;
//            data[12] = 0x30;
        }
        else{

            int currAng = State.Angle;

            //Robots ID
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
        }
    }

	new_comm_ready = true;
}

TVec3 Robot::GetCurrentMotionCommand(void) const
{
	const int motion_idx = static_cast<int>(lastCMDs[10].X);
	return lastCMDs[motion_idx];
}
