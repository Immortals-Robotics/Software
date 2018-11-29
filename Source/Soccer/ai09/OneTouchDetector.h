#include "Geom.h"
#include "../../Common/distance.h"

class OneTouchDetector
{
public:
    float field_w;
    float field_h;
	int hys;
	RobotState* rState;
	BallState* bState;
	int* side;

	float BAR;
	
	OneTouchDetector()
	{
        hys = 0;
		BAR = 89.0;
	}
	bool IsArriving ( float angleTol = 40 , float passAngleLimit = 80 )
	{
		return IsArriving(Vec2(-(*side)*field_w,0),angleTol,passAngleLimit);
	}
	bool IsArriving ( const TVec2& target, float angleTol, float passAngleLimit )
	{
		float ballDistBeforeStop = (bState->velocity.magnitude*bState->velocity.magnitude) / 500.0f;
		//ballDistBeforeStop += 100.0f;
		float angleWithTarget = AngleWith(rState->Position, target);
		if ( ( fabs ( NormalizeAngle ( bState->velocity.direction - AngleWith ( bState->Position , Vec2 ( rState->Position.X + BAR * cosDeg ( angleWithTarget ) , rState->Position.Y + BAR * sinDeg ( angleWithTarget ) ) ) ) ) < angleTol ) && 
			( fabs(NormalizeAngle(180+bState->velocity.direction-AngleWith(rState->Position, target))) < passAngleLimit ) &&
			//( bState->velocity.magnitude > 50 ) )
			( DIS(rState->Position, bState->Position) < ballDistBeforeStop ) )
			//&&(abs(ball.vel_angle-90)>0.01)&&
			//(abs(ball.vel_angle+90)>0.01)&&
			//(abs(ball.vel_angle-180)>0.01)&&
			//(abs(ball.vel_angle+180)>0.01))
		{
			hys = 10;
			return true;
		}
		else if (( hys > 0 )&& ( bState->velocity.magnitude > 50 ))// &&(abs(ball.vel_angle-90)>0.01)&&(abs(ball.vel_angle+90)>0.01)&&(abs(ball.vel_angle-180)>0.01)&&(abs(ball.vel_angle+180)>0.01))
		{
			hys --;
			return true;
		}
		
		else
		{
			hys = 0;
			return false;
		}
	}
};