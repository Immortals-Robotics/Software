#include "ai09.h"

void ai09::NormalPlayDef ( void )
{
	//ManageAttRoles();
	
	MarkManager(false);
	
	for (map<int*,int>::const_iterator i=markMap.begin(); i!=markMap.end(); ++i) {
		int opp = i->second;
		int own = *i->first;
		
		if (0) {//oneTouchDetector[own].IsArriving()) {
			WaitForPass(own, false);
		}
		else
		{
		
			if (opp==-1) {
				OwnRobot[own].face(Vec2(-side*3025, 0));
				ERRTSetObstacles(own, 0, 1, 1, 0, 0, 0);
				if (own==dmf) {
					ERRTNavigate2Point(dmf, Vec2(side*300, -1600), 0, 100, &VELOCITY_PROFILE_MAMOOLI);
				}
				else if (own==mid1) {
					ERRTNavigate2Point(mid1, Vec2(0, 1600), 0, 100, &VELOCITY_PROFILE_MAMOOLI);
				}
				else if (own==mid2) {
					ERRTNavigate2Point(mid2, Vec2(0, -1600), 0, 100, &VELOCITY_PROFILE_MAMOOLI);
				}
			}
			else {
				OwnRobot[own].face(ball.Position);
				if (1){//ball.Position.Y*OppRobot[opp].Position.Y<0) {
					Mark2Goal(own,opp,100);
				}
				else {
					Mark2Ball(own,opp,100);
				}
			}
		}
	}
	
	
	
	/*float shootAngle = NormalizeAngle( 180+calculateOpenAngleToGoal(ball.Position, attack).X);
	debugDraw = true;
	tech_circle(attack, shootAngle, 50, 0, 1, 0, 0, 1);
	debugDraw = false;*/
	
	float shootAngle = NormalizeAngle( 180+calculateOpenAngleToGoal(ball.Position, attack).X);
	float shoot_pow = 1;
	float chip_pow = 500;
	if (DIS(OwnRobot[attack].State.Position,ball.Position) > 400 ) {
		chip_pow = 1;
	}
	if (goal_blocked(ball.Position, 200, 90)) {
		chip_pow = 1;
	}
	
	if (!goal_blocked(ball.Position, 3000, 130)) {
		shoot_pow = 100;
		chip_pow = 0;
	}
	
	if (attackFuckingAngle()) {
		shootAngle = AngleWith(ball.Position, Vec2(side*3025, 0));
		shoot_pow = 0;
		chip_pow = 500;
	}
	
	debugDraw = true;
	tech_circle(attack, shootAngle, shoot_pow, chip_pow, 1, 0, 0, 1);
	//circle_ball(attack, 90, 80, 0, 1.0f);
	debugDraw = false;
}