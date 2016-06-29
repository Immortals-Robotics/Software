#include "ai09.h"

void ai09::Mark2Goal(int robot_num, int opp, float dist)
{
	const float opp_predict_t = 0.15;
	dist = min(1500, dist);

	TVec2 predictedOpp = Vec2(OppRobot[opp].Position.X + OppRobot[opp].velocity.x*opp_predict_t, OppRobot[opp].Position.Y + OppRobot[opp].velocity.y*opp_predict_t);
	TVec2 target = PointOnConnectingLine(predictedOpp, Vec2(side*field_width, 0), dist);
	
	OwnRobot[robot_num].face(ball.Position);
	ERRTSetObstacles(robot_num, 1, 1, 1, 1, 0, 0);
	ERRTNavigate2Point(robot_num, target, 0, 100, &VELOCITY_PROFILE_KHARAKI);
}