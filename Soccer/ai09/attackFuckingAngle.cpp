#include "ai09.h"

bool ai09::attackFuckingAngle ( void )
{
	bool ans = false;
	
	static int attfuanHys = 0;
	
	int oppAttack = findKickerOpp(-1);
	if (oppAttack != -1) {
		float ownGoalAngle = AngleWith(Vec2(side*field_width, 0),ball.Position);
		float oppGoalAngle = AngleWith(ball.Position, Vec2(-side*field_height, 0));
		
		if (fabs(NormalizeAngle(ownGoalAngle-oppGoalAngle)) > 80) {
			ans = true;
		}
	}
	
	if (ans) {
		attfuanHys = 30;
		return true;
	}
	
	
	if (attfuanHys > 0) {
		attfuanHys --;
		return true;
	}
	
	return false;
}