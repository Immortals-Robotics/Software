#include "ai09.h"

void ai09::NormalPlayAtt ( void )
{
	if (oneTouchDetector[dmf].IsArriving(70)) {
		WaitForPass ( dmf );
	}
	else {
		ERRTSetObstacles ( dmf , false , true , true , true );
		OwnRobot[dmf].face(ball.Position);
		ERRTNavigate2Point ( dmf , PointOnConnectingLine(ball.Position, Vec2(side*3025, 0), 1500) ,0 , 100,&VELOCITY_PROFILE_MAMOOLI);
	}
	
	
	if ( ( ball.Position.X * side < -1500 ) && ( fabs ( ball.Position.Y ) > 1400 ) )
	{
		float passAngle = AngleWith ( Vec2 ( -side*1700 , -sgn ( ball.Position.Y ) * 1700 ) , ball.Position );
		tech_circle(attack, passAngle, 7, 0, 1, 1, 0, 1);
		
		if (oneTouchDetector[lmf].IsArriving(70)) {
			WaitForPass ( lmf );
		}
		else {
			OwnRobot[lmf].face ( Vec2 ( -side*3033 , 0 ) );
			ERRTSetObstacles ( lmf );
			ERRTNavigate2Point ( lmf , Vec2 ( -side*(1250-sgn(ball.Position.Y)*750) , 1700 ) );
		}
		
		if (oneTouchDetector[rmf].IsArriving(70)) {
			//WaitForPass ( rmf );
			WaitForOmghi(rmf);
		}
		else {
			OwnRobot[rmf].face ( Vec2 ( -side*3033 , 0 ) );
			ERRTSetObstacles ( rmf );
			ERRTNavigate2Point ( rmf , Vec2 ( -side*(1250-sgn(ball.Position.Y)*750) , -1700 ) );
		}
	}
	else {
		float shootAngle = NormalizeAngle( 180+calculateOpenAngleToGoal(ball.Position, attack).X);
		tech_circle(attack, shootAngle, 15, 0, 1, 0, 0, 1);
		
		if (oneTouchDetector[lmf].IsArriving(70)) {
			WaitForPass ( lmf );
		}
		else {
			OwnRobot[lmf].face ( Vec2 ( -side*3033 , 0 ) );
			ERRTSetObstacles ( lmf );
			ERRTNavigate2Point ( lmf , Vec2 ( -side*(1250-sgn(ball.Position.Y)*750) , 1700 ) );
		}
		
		if (oneTouchDetector[rmf].IsArriving(70)) {
			//WaitForPass ( rmf );
			WaitForOmghi(rmf);
		}
		else {
			OwnRobot[rmf].face ( Vec2 ( -side*3033 , 0 ) );
			ERRTSetObstacles ( rmf );
			ERRTNavigate2Point ( rmf , Vec2 ( -side*(1250-sgn(ball.Position.Y)*750) , -1700 ) );
		}
	}
	
}