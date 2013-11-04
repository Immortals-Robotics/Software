#include "ai09.h"

void ai09::NormalPlayAtt ( void )
{
	ManageAttRoles ( );
	
	
	if (oneTouchDetector[dmf].IsArriving(70)) {
		WaitForPass ( dmf );
	}
	else {
		ERRTSetObstacles ( dmf , false , true , true , true );
		OwnRobot[dmf].face(ball.Position);
		ERRTNavigate2Point ( dmf , PointOnConnectingLine(ball.Position, Vec2(side*3025, 0), 2500) ,0 , 100,&VELOCITY_PROFILE_MAMOOLI);
	}
	
	
	if ( ( ball.Position.X * side < -1500 ) && ( fabs ( ball.Position.Y ) > 1400 ) )
	{
		float passAngle = AngleWith ( Vec2 ( -side*1700 , -sgn ( ball.Position.Y ) * 1700 ) , ball.Position );
		tech_circle(attack, passAngle, 0, 35, 1, 1, 0, 1);
	}
	else {
		float shootAngle = NormalizeAngle( 180+calculateOpenAngleToGoal(ball.Position, attack).X);
		tech_circle(attack, shootAngle, 50, 0, 1, 0, 0, 1);
	}

	const float openAngleTresh = 6;
		
		if (oneTouchDetector[mid1].IsArriving(70)) {
			TVec2* target = NULL;
			float openAngle = calculateOpenAngleToGoal(OwnRobot[mid1].State.Position, mid1).Y;
			if (openAngle <openAngleTresh)// ( OwnRobot[mid1].State.Position.X* side < -2300 ) && ( fabs ( OwnRobot[mid1].State.Position.Y ) > 1000 ) ) 
			{
				target = new TVec2;
				*target = OwnRobot[mid2].State.Position;
			}
			WaitForPass ( mid1 , (target==NULL)?false:true , target );
		}
		else {
			OwnRobot[mid1].face ( Vec2 ( -side*3033 , 0 ) );
			ERRTSetObstacles ( mid1 );
			ERRTNavigate2Point ( mid1 , Vec2 ( -side*(1250-sgn(ball.Position.Y)*750) , 1700 ) );
		}
		
		if (oneTouchDetector[mid2].IsArriving(70)) {
			TVec2* target = NULL;
			float openAngle = calculateOpenAngleToGoal(OwnRobot[mid2].State.Position, mid2).Y;
			if (openAngle<openAngleTresh)// ( OwnRobot[mid2].State.Position.X* side < -2300 ) && ( fabs ( OwnRobot[mid2].State.Position.Y ) > 1000 ) ) 
			{
				target = new TVec2;
				*target = OwnRobot[mid1].State.Position;
			}
			WaitForPass ( mid2 , (target==NULL)?false:true , target );
			//WaitForOmghi(mid2);
		}
		else {
			OwnRobot[mid2].face ( Vec2 ( -side*3033 , 0 ) );
			ERRTSetObstacles ( mid2 );
			ERRTNavigate2Point ( mid2 , Vec2 ( -side*(1250+sgn(ball.Position.Y)*750) , -1700 ) );
		}
}