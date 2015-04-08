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
		
		//if ( robot_num == 0 )
		{
			/*for ( int i = 0 ; i < planner[robot_num].GetWayPointNum() - 1 ; i ++ )
			{
				Debug_Line * newDbgLine = AIDebug.add_line();
				newDbgLine -> set_x1(planner[robot_num].GetWayPoint(i).X);
				newDbgLine -> set_y1(planner[robot_num].GetWayPoint(i).Y);
				newDbgLine -> set_x2(planner[robot_num].GetWayPoint(i+1).X);
				newDbgLine -> set_y2(planner[robot_num].GetWayPoint(i+1).Y);
			}*/
			
		}
		
		//if ( planner[robot_num].GetWayPointNum ( ) <= 2 )
			Navigate2Point ( robot_num , wayp , accurate , speed , velocityProfile );
		//else
		//	Navigate2Point ( robot_num , wayp , false , speed , velocityProfile );
		//Navigate2Point ( robot_num , dest , accurate , speed );
	}
}