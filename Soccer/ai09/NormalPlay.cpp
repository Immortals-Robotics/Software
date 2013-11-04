#include "ai09.h"
#include <iostream>
using namespace std;

void ai09::NormalPlay ( void )
{
	GK ( 0 , 1 );
	OneDef ( 1 );

	ERRTSetObstacles ( 2 , true , true , true , true );
	OwnRobot[2].face(Vec2(ball.Position.X,ball.Position.Y));
	ERRTNavigate2Point ( 2 , CircleAroundPoint(Vec2(ball.Position.X,ball.Position.Y),NormalizeAngle(-20+AngleWith(ball.Position , Vec2(side*3025,0))),590) ,0 , 100);

	ERRTSetObstacles ( 3 , true , true , true , true );
	OwnRobot[3].face(Vec2(ball.Position.X,ball.Position.Y));
	ERRTNavigate2Point ( 3 , CircleAroundPoint(Vec2(ball.Position.X,ball.Position.Y),NormalizeAngle(20+AngleWith(ball.Position , Vec2(side*3025,0))),590) ,0 , 100);
}