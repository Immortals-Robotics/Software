#include "ai09.h"
#include <math.h>

#define world2mapX(a) min(729,max(0,((a/10)+364)))
#define world2mapY(a) min(529,max(0,((a/10)+264)))

#define maxgkAngle 65
static int my_hys=0;
void ai09::GK ( int robot_num , int defence_num , bool stop )
{
	Line ball_line = Line::makeLineFromPositionAndAngle ( VecPosition ( ball.Position.X , ball.Position.Y ) , ball.velocity.direction );
	Line targetLine = Line::makeLineFromTwoPoints ( VecPosition ( side * 3025 , -100 ) , VecPosition ( side * 3025 , 100 ) );
	VecPosition ballInter = ball_line.getIntersection(targetLine);
	 //cout << ballInter.getX()<<"	"<<ballInter.getY();
	cout << "ANG:	BALL: " << AngleWith(Vec2(side*3025, 0),ball.Position) << "		";
	cout << "GK: " << AngleWith(Vec2(side*3025, 0),OwnRobot[gk].State.Position) << "		";
	cout << "DEF: " << AngleWith(Vec2(side*3025, 0),OwnRobot[def].State.Position) << endl;
	
	cout << "DIS:	BALL: " << DIS(Vec2(side*3025, 0),ball.Position) << "		";
	cout << "GK: " << DIS(Vec2(side*3025, 0),OwnRobot[gk].State.Position) << "		";
	cout << "DEF: " << DIS(Vec2(side*3025, 0),OwnRobot[def].State.Position) << endl;



	if ((fabs(ballInter.getY())<450)&&(ball.velocity.magnitude>1000)&&(fabs(ball.velocity.direction)>90))
	{
		GK_shirje(robot_num);
		my_hys = 30;
	}
	else if ( my_hys > 0 )
	{
		GK_shirje(robot_num);
		my_hys --;
	}
	
	else
	{
		OwnRobot[robot_num].Chip(15);

		my_hys = 0;
		
	ERRTSetObstacles ( robot_num , false , true , false , false , false );
	if ( ( IsInObstacle ( Vec2 ( world2mapX(ball.Position.X),world2mapY(ball.Position.Y) ) ) ) && ( ball.velocity.magnitude < 1000 ) && (!stop) )
	{
		ERRTSetObstacles ( robot_num , 0 , 0 , 1 , 0 , 0 );
		//tech_circle(robot_num,sgn(ball.Position.Y)*side*60 ,0,15,false);
		tech_circle(robot_num,AngleWith ( ball.Position , Vec2 ( side * 3133 , 0 ) ) ,0,9,false,0,0,0);
	}
	else
	{
		//ERRTSetObstacles ( robot_num , false , false , true , true );
		float goalAngle = AngleWith(Vec2(side*3025, 0),ball.Position);
		if ( side == 1 )
		{
			goalAngle += 180.0f;
			goalAngle = NormalizeAngle(goalAngle);
			
			goalAngle = min(maxgkAngle, max(-maxgkAngle,goalAngle));
			
			goalAngle += 180.0f;
			goalAngle = NormalizeAngle(goalAngle);
		}
		else
		{
			goalAngle = min(maxgkAngle, max(-maxgkAngle,goalAngle));
		}
		
		
		OwnRobot[robot_num].face ( Vec2 ( ball.Position.X , ball.Position.Y ) );
		if ( defence_num == 2 )
		{
			if ( stop )
			{
				ERRTSetObstacles(robot_num, 1, 0, 1, 1, 0);
				ERRTNavigate2Point(robot_num, Vec2(side*3025+cosDeg(goalAngle)*300.0f, sinDeg(goalAngle)*300.0f));
			}
			else
			{
				//Navigate2Point ( robot_num , PointOnConnectingLine(Vec2(side*3025.0f,0),Vec2(ball.Position.X,ball.Position.Y),300.0f) );
				Navigate2Point(robot_num, Vec2(side*3025+cosDeg(goalAngle)*300.0f, sinDeg(goalAngle)*300.0f));
			}
		}
		else
		{
			if ( stop )
			{
				ERRTSetObstacles(robot_num, 1, 0, 1, 1, 0);
				ERRTNavigate2Point ( robot_num , PointOnConnectingLine(Vec2(side*3025.0f,100.0f*sgn(ball.Position.Y+100.0f)),Vec2(ball.Position.X,ball.Position.Y),400.0f) );
			}
			else
			{
				Navigate2Point ( robot_num , PointOnConnectingLine(Vec2(side*3025.0f,100.0f*sgn(ball.Position.Y+100.0f)),Vec2(ball.Position.X,ball.Position.Y),400.0f) );
			}
		}
	}
	}
}