#include "ai09.h"

void ai09::Navigate2Point ( int robot_num , TVec2 dest , bool accurate , int speed , VelocityProfile * velocityProfile )
{
	OwnRobot[robot_num].target.Position.X = dest.X;
	OwnRobot[robot_num].target.Position.Y = dest.Y;
	
	if ( velocityProfile == NULL )
		velocityProfile = &this->VELOCITY_PROFILE_MAMOOLI;
	
	OwnRobot[robot_num].Move ( accurate , speed , velocityProfile );
	
	navigated[robot_num] = true;
}

void ai09::ERRTNavigate2Point ( int robot_num , TVec2 dest , bool accurate , int speed , VelocityProfile * velocityProfile )
{
    //Navigate2Point(robot_num, dest,accurate,speed,velocityProfile);
    //return;
	if ( OwnRobot[robot_num].State.seenState == CompletelyOut )
		Halt ( robot_num );
	else
	{
		planner[robot_num].init ( OwnRobot[robot_num].State.Position , dest , 90.0f );
		TVec2 wayp = planner[robot_num].Plan ( );
		TVec2 v;

		if ( planner[robot_num].GetWayPointNum ( ) <= 2 )
			v = Vec2(0);
		else
			v = Normalize(wayp - OwnRobot[robot_num].State.Position) * 500.0f;
		OwnRobot[robot_num].target.velocity.x = v.X;
		OwnRobot[robot_num].target.velocity.y = v.Y;
		Navigate2Point(robot_num, wayp, accurate, speed, velocityProfile);
	}
}