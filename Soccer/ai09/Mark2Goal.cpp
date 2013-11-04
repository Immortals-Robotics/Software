#include "ai09.h"

void ai09::Mark2Goal ( int robot_num , int opp , float dist )
{
	TVec2 target = PointOnConnectingLine ( OppRobot[opp].Position , Vec2 ( side*2995 , 0 ) , dist );
	ERRTSetObstacles(robot_num, 0, 1, 1, 0, 0, 0);
	ERRTNavigate2Point(robot_num, target, 0, 100, &VELOCITY_PROFILE_MAMOOLI);
}