#include "ai09.h"
#include <iostream>
using namespace std;

void ai09::Stop ( int gk , int def , int wall1 , int wall2 , int wall3 )
{
	GK ( gk , 1 );
	OneDef ( def , NULL , true );
	//TwoDef ( def , wall1 );

	ERRTSetObstacles ( wall1 , true , true , true , true );
	OwnRobot[wall1].face(Vec2(ball.Position.X,ball.Position.Y));
	ERRTNavigate2Point ( wall1 , CircleAroundPoint(Vec2(ball.Position.X,ball.Position.Y),NormalizeAngle(-20+AngleWith(ball.Position , Vec2(side*3025,0))),590) ,0 , 40,&VELOCITY_PROFILE_AROOM);	
	
	ERRTSetObstacles ( wall2 , true , true , true , true );
	OwnRobot[wall2].face(Vec2(ball.Position.X,ball.Position.Y));
	ERRTNavigate2Point ( wall2 , CircleAroundPoint(Vec2(ball.Position.X,ball.Position.Y),NormalizeAngle(20+AngleWith(ball.Position , Vec2(side*3025,0))),590) ,0 , 40,&VELOCITY_PROFILE_AROOM);

	ERRTSetObstacles ( wall3 , true , true , true , true );
	OwnRobot[wall3].face(Vec2(ball.Position.X,ball.Position.Y));
	ERRTNavigate2Point ( wall3 , CircleAroundPoint(Vec2(ball.Position.X,ball.Position.Y),NormalizeAngle(AngleWith(ball.Position , Vec2(side*3025,0))),580) ,0 , 40,&VELOCITY_PROFILE_AROOM);
}