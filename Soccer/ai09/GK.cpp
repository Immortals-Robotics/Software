#include "ai09.h"
#include <math.h>

#define world2mapX(a) min(729,max(0,((a/10)+364)))
#define world2mapY(a) min(529,max(0,((a/10)+264)))

#define maxgkAngle 65

void ai09::GK ( int robot_num , int defence_num )
{
	ERRTSetObstacles ( robot_num , false , true , false , false , false );
	if ( ( IsInObstacle ( Vec2 ( world2mapX(ball.Position.X),world2mapY(ball.Position.Y) ) ) ) && ( ball.velocity.magnitude < 300 ) )
	{
		ERRTSetObstacles ( robot_num , 0 , 0 , 1 , 0 , 0 );
		//tech_circle(robot_num,sgn(ball.Position.Y)*side*60 ,0,15,false);
		tech_circle(robot_num,AngleWith ( ball.Position , Vec2 ( side * 3033 , 0 ) ) ,0,15,false);
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
			//Navigate2Point ( robot_num , PointOnConnectingLine(Vec2(side*3025.0f,0),Vec2(ball.Position.X,ball.Position.Y),300.0f) );
			Navigate2Point(robot_num, Vec2(side*3025+cosDeg(goalAngle)*300.0f, sinDeg(goalAngle)*300.0f));
		}
		else
		{
			Navigate2Point ( robot_num , PointOnConnectingLine(Vec2(side*3025.0f,100.0f*sgn(ball.Position.Y+100.0f)),Vec2(ball.Position.X,ball.Position.Y),400.0f) );
		}
	}
}