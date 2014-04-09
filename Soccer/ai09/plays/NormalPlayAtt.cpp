#include "ai09.h"

void ai09::NormalPlayAtt ( void )
{
	ManageAttRoles ( );
	
	debugDraw=true;
	recievePass(dmf,PointOnConnectingLine(ball.Position, Vec2(side*3025, 0), 2500));
	debugDraw=false;
	
	if (oneTouchType[attack]==allaf) {
		ERRTSetObstacles ( attack , false , true , true , true );
		OwnRobot[attack].face(Vec2(-side*3025, 0));
		//OwnRobot[robot_num].target.Angle=-90;
		ERRTNavigate2Point ( attack , allafPos[attack] ,0 , 100,&VELOCITY_PROFILE_MAMOOLI);
		if (timer.time()>2.5) {
			oneTouchType[attack] = oneTouch;
		}
	}
	else if ( ( ball.Position.X * side < -1300 ) && ( fabs ( ball.Position.Y ) > 1400 ) )
	{
		float passAngle = AngleWith ( Vec2 ( -side*1700 , -sgn ( ball.Position.Y ) * 1700 ) , ball.Position );
		tech_circle(attack, passAngle, 0, 89, 1, 1, 0, 1);
	}
	else {
		float shootAngle = NormalizeAngle( 180+calculateOpenAngleToGoal(ball.Position, attack).X);
		float shoot_pow = 140;
		if (DIS(OwnRobot[attack].State.Position,ball.Position) > 400 ) {
			shoot_pow = 1;
		}
		if (goal_blocked(ball.Position, 200, 90)) {
			shoot_pow = 1;
		}
		
		if (attackFuckingAngle()) {
			shootAngle = AngleWith(ball.Position, Vec2(side*3025, 0));
			shoot_pow = 1;
		}
		
		debugDraw = true;
		tech_circle(attack, shootAngle, shoot_pow, 0, 1, 0, 0, 1);
		//circle_ball(attack, 90, 80, 0, 1.0f);
		debugDraw = false;
	}

	if (ball.Position.Y>600) {
		recievePass(mid1, Vec2 ( -side*250 , 0 ));
	}
	else {
		recievePass(mid1, Vec2 ( -side*(2250) , 1500 ));
	}
	
	if (ball.Position.Y<-600) {
		recievePass(mid2, Vec2 ( -side*250 , 0 ));
	}
	else {
		recievePass(mid2, Vec2 ( -side*(2250) ,-1500 ));
	}

}