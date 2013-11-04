#include "ai09.h"

void ai09::backPass ( int robot_num , float target )
{
	static TVec2 initBall;
	timer.end();
	if ( timer.time() < 1 )
	{
		initBall = ball.Position;
		tech_circle(robot_num, 90 - side * 90 , 0, 0,true);
	}
	else if ( timer.time() < 2 )
	{
		tech_circle(robot_num, 180 + target , 0, 0,true);
	}
	else if ( timer.time() < 2.8 )
	{
		tech_circle(robot_num, 180 + target , 0,1,true);
	}
	else {
		OwnRobot[robot_num].Dribble(15);
		OwnRobot[robot_num].target.Angle = target;
		Navigate2Point(robot_num, Vec2(initBall.X+cosDeg(target+210)*800, initBall.Y+sinDeg(target+210)*800));
	}
}