#include "ai09.h"

void ai09::Halt ( int robot_num )
{
	//OwnRobot[robot_num].MoveByMotion(Vec3(0.0f));
	OwnRobot[robot_num].target.Angle = OwnRobot[robot_num].State.Angle;
	Navigate2Point(robot_num, OwnRobot[robot_num].State.Position, 0, 0);
	//OwnRobot[robot_num].data[9] = 110;
	OwnRobot[robot_num].halted = true;
	
	navigated[robot_num] = true;
}

void ai09::HaltAll ( void )
{
	for ( int i = 0 ; i < Setting::kMaxOnFieldTeamRobots; i ++ )
	{
		Halt(i);
	}
}