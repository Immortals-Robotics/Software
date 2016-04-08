#include "ai09.h"

void ai09::Mark(int robot_num, int opp, float dist)
{
	auto oppToBall = Normalize(ball.Position - OppRobot[opp].Position);
	auto oppToGoal = Normalize(Vec2(side*field_width, 0) - OppRobot[opp].Position);
	auto oppToGoalDis = DIS(OppRobot[opp].Position, Vec2(side*field_width, 0));
	auto ballToOppDis = DIS(ball.Position, OppRobot[opp].Position);
	auto oneTouchDot = Dot(oppToBall, oppToGoal);
	if (oneTouchDot > 0 || oppToGoalDis < 2500) {
		Mark2Goal(robot_num, opp, dist);
	}
	else {
		if (oneTouchDetector[robot_num].IsArriving(20)) {
			WaitForPass(robot_num, false);
		}
		else
		{
			Mark2Ball(robot_num, opp, dist);
		}
	}
}
