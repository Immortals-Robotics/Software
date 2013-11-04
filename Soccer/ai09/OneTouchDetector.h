#include "WorldState.h"
#include "Geom.h"

class OneTouchDetector
{
public:
	int hys;
	RobotState* rState;
	BallState* bState;
	float BAR;
	
	OneTouchDetector(){hys = 0;}
	bool IsArriving ( float angleTol = 110 )
	{
		if ( ( fabs ( NormalizeAngle ( bState->velocity.direction - AngleWith ( bState->Position , Vec2 ( rState->Position.X + BAR * cosDeg ( rState->Angle ) , rState->Position.Y + BAR * sinDeg ( rState->Angle ) ) ) ) ) < 70 ) && ( fabs(NormalizeAngle(180+bState->velocity.direction-rState->Angle)) < 100 ) && ( bState->velocity.magnitude > 50 ) )//&&(abs(ball.vel_angle-90)>0.01)&&(abs(ball.vel_angle+90)>0.01)&&(abs(ball.vel_angle-180)>0.01)&&(abs(ball.vel_angle+180)>0.01))
		{
			hys = 30;
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