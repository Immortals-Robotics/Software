#include "ai09.h"

void ai09::backPass ( int robot_num , float target , float t )
{
	static TVec2 initBall;
	float dt = timer.time()-t;
	if ( dt < 0 )
	{
		initBall = ball.Position;
		tech_circle(robot_num, 90 + side * 90 , 0, 0,true,1,0,1);
	}
	else if ( dt < 1 )
	{
		tech_circle(robot_num,  180+target , 0, 0,true,1,0,1);
	}
	else if ( dt < 2 )
	{
		tech_circle(robot_num,  180+target , 0,0,true,1,1,1);
	}
	else {
		//OwnRobot[robot_num].Dribble(15);
		OwnRobot[robot_num].target.Angle = target;
		Navigate2Point(robot_num, Vec2(initBall.X+cosDeg(target+210)*800, initBall.Y+sinDeg(target+210)*800),0,100,&VELOCITY_PROFILE_KHARAKI);
	}
}