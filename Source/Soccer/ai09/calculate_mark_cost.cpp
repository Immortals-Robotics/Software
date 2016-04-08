#include "ai09.h"

float ai09::calculateMarkCost(int robot_num, int opp)
{
	if (OwnRobot[robot_num].State.seenState == CompletelyOut)
		return -1;
	if (OppRobot[opp].seenState == CompletelyOut)
		return -1;

	const float predict_t = 0.3f;
	auto predicted_pos_own = Vec2(
		OwnRobot[robot_num].State.Position.X + OwnRobot[robot_num].State.velocity.x*predict_t,
		OwnRobot[robot_num].State.Position.Y + OwnRobot[robot_num].State.velocity.y*predict_t);
	auto predicted_pos_opp = Vec2(
		OppRobot[opp].Position.X + OppRobot[opp].velocity.x*predict_t,
		OppRobot[opp].Position.Y + OppRobot[opp].velocity.y*predict_t);

	auto dis_pred = DIS(predicted_pos_own, predicted_pos_opp);
	bool already_marked = false;
	for (auto it = markMap.begin(); it != markMap.end(); ++it) {
		if (*it->first == robot_num)
		{
			already_marked = it->second == opp;
			break;
		}
	}

	float score_stay;
	if (!already_marked)
		score_stay = 0.0f;
	else
	{
		if (dis_pred < 500)
			score_stay = 1.0f;
		else
			score_stay = 1.0f - (dis_pred - 500.0f) / 1000.0f;
	}

	float cost_reach;
	if (dis_pred < 500)
		cost_reach = 0.0f;
	else
		cost_reach = (dis_pred - 500.0f) / 1000.0f;

	float cost_attack = robot_num == attack ? 1.0f : 0.0f;

	score_stay = min(1.0f, max(0.0f, score_stay));
	cost_reach = min(1.0f, max(0.0f, cost_reach));
	cost_attack = min(1.0f, max(0.0f, cost_attack));

	auto cost = 1.0f - score_stay + cost_reach + cost_attack;

	return cost;
}
