#include "ai09.h"

void ai09::WaitForOmghi ( int robot_num )
{
	Line ball_line = Line::makeLineFromPositionAndAngle ( VecPosition ( ball.Position.X , ball.Position.Y ) , ball.velocity.direction );
	VecPosition ans = ball_line.getPointOnLineClosestTo ( VecPosition ( OwnRobot[robot_num].State.Position.X + BAR * cosDeg ( OwnRobot[robot_num].State.Angle ) , OwnRobot[robot_num].State.Position.Y + BAR * sinDeg ( OwnRobot[robot_num].State.Angle ) ) );
	
	bool moving = true;
	float sBAR;
	if ( moving )
	{
		BAR = 83;
		ans = ball_line.getPointOnLineClosestTo ( VecPosition ( OwnRobot[robot_num].State.Position.X + BAR * cosDeg ( OwnRobot[robot_num].State.Angle ) , OwnRobot[robot_num].State.Position.Y + BAR * sinDeg ( OwnRobot[robot_num].State.Angle ) ) );
		sBAR = ans.getDistanceTo ( VecPosition ( ball.Position.X , ball.Position.Y ) );
		sBAR /= ball.velocity.magnitude;
		sBAR = ans.getDistanceTo ( VecPosition ( OwnRobot[robot_num].State.Position.X , OwnRobot[robot_num].State.Position.Y ) ) / sBAR;
		sBAR /= 55.0;
		//sBAR /= 1500000;
		if ( sBAR < 20 )		sBAR = 20;
		if ( sBAR > 100 )	sBAR = 100;
	}	
	//sBAR = min ( ans.getDistanceTo ( VecPosition ( OwnRobot[robot_num].State.Position.X , OwnRobot[robot_num].State.Position.Y ) ) , BAR );
	
	/*if ( ball.velocity.magnitude < 1500 )//DIS ( OwnRobot[robot_num].State.Position , ball.Position ) < 40 )
	 {
	 ERRTSetObstacles ( robot_num );
	 //tech_circle(attack,AngleWith ( Vec2 ( -side * 3033 , 0 ) , ball.Position ) ,15);
	 tech_circle(robot_num, NormalizeAngle( 180+calculateOpenAngleToGoal(ball.Position, robot_num).X), 15 );
	 return;
	 }
	 else
	 OwnRobot[robot_num].face ( ball.Position );*/
	OwnRobot[robot_num].target.Angle = calculateOneTouchAngle ( robot_num , Vec2 (ans.getX() - BAR * cosDeg ( OwnRobot[robot_num].State.Angle ) , ans.getY() - BAR * sinDeg ( OwnRobot[robot_num].State.Angle ) ) );
	
	
	ERRTSetObstacles ( robot_num );
	if ( moving )
	{
		BAR = -200;
		ERRTNavigate2Point ( robot_num , Vec2 (ans.getX() - BAR * cosDeg ( OwnRobot[robot_num].State.Angle ) , ans.getY() - BAR * sinDeg ( OwnRobot[robot_num].State.Angle ) ) , 0 , sBAR , &VELOCITY_PROFILE_KHARAKI);
	}
	else
		ERRTNavigate2Point ( robot_num , Vec2 (ans.getX() - BAR * cosDeg ( OwnRobot[robot_num].State.Angle ) , ans.getY() - BAR * sinDeg ( OwnRobot[robot_num].State.Angle ) ) );
	
	//if ( fabs ( NormalizeAngle ( OwnRobot[robot_num].State.Angle - AngleWith ( OwnRobot[robot_num].State.Position , Vec2 ( -side*3025 , 0 ) ) ) ) < 20 ) 
	OwnRobot[robot_num].Shoot( 15 );
	//else
	//OwnRobot[robot_num].Shoot( 0 );
	
	OwnRobot[robot_num].Dribble( 15 );
}