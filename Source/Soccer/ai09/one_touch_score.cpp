#include "ai09.h"

float ai09::oneTouchScore ( int robot_num )
{
	TVec2 oneTouchPos = CalculatePassPos(robot_num,Vec2(-side*3025, 0),OwnRobot[robot_num].State.Position, 95);
	float travelTime = calculateRobotReachTime(robot_num, oneTouchPos, &VELOCITY_PROFILE_MAMOOLI);
	TVec2 ballPosAtReach = predictBallForwardAI ( travelTime );
	return 1.0;
}