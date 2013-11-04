#include "ai09.h"
#include <math.h>

void ai09::dribble ( int robot_num , TVec2 target )
{
	ERRTSetObstacles ( robot_num , false , true , true , false );
	OwnRobot[robot_num].face ( ball.Position );
	float appAng = AngleWith(OwnRobot[robot_num].State.Position, ball.Position);
	appAng /=1.3;
	TVec2 exte = Vec2(cosDeg(appAng)*(DIS(OwnRobot[robot_num].State.Position, ball.Position)+300.0f), sinDeg(appAng)*(DIS(OwnRobot[robot_num].State.Position, ball.Position)+300.0f));
	exte += OwnRobot[robot_num].State.Position;
	// = PointOnConnectingLine(OwnRobot[robot_num].State.Position, ball.Position, DIS(OwnRobot[robot_num].State.Position, ball.Position)+3300.0f);
	ERRTNavigate2Point ( robot_num , exte , 0 , 70 );
	OwnRobot[robot_num].Dribble(15);
}