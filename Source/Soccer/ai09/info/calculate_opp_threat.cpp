#include "ai09.h"

float ai09::calculateOppThreat(int opp, bool restart)
{
	if (OppRobot[opp].seenState == CompletelyOut)
		return -1;
	if (opp == oppGK)
		return -1;

	if ((DIS(OppRobot[opp].Position, ball.Position) < 400) &&
		((DIS(OwnRobot[attack].State.Position, ball.Position) < 400) ||
		(restart)))
		return -1;

	if (OppRobot[opp].Position.X*side < -1500 && fabs(OppRobot[opp].Position.X - ball.Position.X) > 2000)
		return 0;


	float oppDisToGoal = DIS(OppRobot[opp].Position, Vec2(side*field_width, 0));

	TVec2 t2 = Vec2(field_width*side, goal_width / 2.0);
	TVec2 t1 = Vec2(field_width*side, -goal_width / 2.0);
	float t1Angel = atan2((t1.Y - OppRobot[opp].Position.Y), (t1.X - OppRobot[opp].Position.X));
	float t2Angel = atan2((t2.Y - OppRobot[opp].Position.Y), (t2.X - OppRobot[opp].Position.X));
	float oppOpenAngleToGoal = fabs(NormalizeAngle(t2Angel - t1Angel))*180.0f / 3.1415f;

	auto oppToBall = Normalize(ball.Position - OppRobot[opp].Position);
	auto oppToGoal = Normalize(Vec2(side*field_width, 0) - OppRobot[opp].Position);
	auto oneTouchDot = Dot(oppToBall, oppToGoal);

	auto ballToOppDis = DIS(ball.Position, OppRobot[opp].Position);

	float score_goal_dis;
	if (oppDisToGoal < 4000)
		score_goal_dis = 1.0f;
	else
		score_goal_dis = 1.0f - pow(max(0.0f, (oppDisToGoal - 4000.0f) / 3000.0f), 0.5f);

	float score_ball_dis;
	if (ballToOppDis < 1500)
		score_ball_dis = pow(ballToOppDis / 1500.0f, 2.0f);
	else if (ballToOppDis < 4000)
		score_ball_dis = 1.0f;
	else
		score_ball_dis = 1.0f - (ballToOppDis - 4000.0f) / 4000.0f;

	float score_open_angle = oppOpenAngleToGoal / 15.0f;

	float score_one_touch_angle;
	if (oneTouchDot > 0.7f)
		score_one_touch_angle = 1.0f - (oneTouchDot - 0.7f) / 0.3f;
	else if (oneTouchDot > 0.0f)
		score_one_touch_angle = 1.0f;
	else
		score_one_touch_angle = 0.0f;

	score_goal_dis = min(1.0f, max(0.0f, score_goal_dis));
	score_ball_dis = min(1.0f, max(0.0f, score_ball_dis));
	score_open_angle = min(1.0f, max(0.0f, score_open_angle));
	score_one_touch_angle = min(1.0f, max(0.0f, score_one_touch_angle));

	auto final_score_one_touch = score_one_touch_angle * min(score_ball_dis*score_goal_dis, score_open_angle);
	auto final_score_turn_shoot = min(score_ball_dis*score_goal_dis, score_open_angle);

	auto score = max(final_score_one_touch, final_score_turn_shoot);

	return score;
}
