#include "ai09.h"

void ai09::WaitForGool ( int robot_num , bool chip )
{
	TVec2 pos = CalculatePassPos(robot_num,Vec2(-side*3025, 0),OwnRobot[robot_num].State.Position,-1600);
	
	OwnRobot[robot_num].face(Vec2(-side*field_width, 0));
	
	ERRTSetObstacles ( robot_num ,0,1,1,0,0,0);
	ERRTNavigate2Point ( robot_num ,pos,0,100,&VELOCITY_PROFILE_MAMOOLI );
	
	OwnRobot[robot_num].Shoot( 100 );
	OwnRobot[robot_num].Dribble( 15 );
}