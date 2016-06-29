#include "ai09.h"

float ai09::calculateSwicthToAttackerScore(int robot_num)
{
	if (OwnRobot[robot_num].State.seenState == CompletelyOut)
		return -1;

	if (robot_num != mid1 && robot_num != mid2)
		return -1;

	if (!isDefending && oneTouchDetector[robot_num].IsArriving(45, 150))
		return -1;

	float currAttBallDis = DIS(OwnRobot[robot_num].State.Position, ball.Position);

	if (OwnRobot[attack].State.seenState == CompletelyOut)
		currAttBallDis = 10000;

	auto marked_id = -1;
	for (auto it = markMap.begin(); it != markMap.end(); ++it)
	{
		if (*it->first == robot_num)
		{
			marked_id = it->second;
			break;
		}
	}

	if (isDefending && marked_id != -1)
	{
		auto opp = marked_id;
		if (
			(DIS(OppRobot[opp].Position, ball.Position) < 400) &&
			(DIS(OwnRobot[robot_num].State.Position, ball.Position) < 400) &&
			(currAttBallDis > 600) &&
			(ball.velocity.magnitude < 500)) {
			return 0;
		}
		else
			return -1;
	}

	auto disToBall = DIS(OwnRobot[robot_num].State.Position, ball.Position);
	if (disToBall > currAttBallDis - 500)
		return 0;

	auto dis_score = (currAttBallDis - disToBall - 500) / 1000.0f;
	dis_score = min(1.0f, max(0.0f, dis_score));

	return dis_score;
}
