#include "ai09.h"
#include <iostream>
using namespace std;

void ai09::stop_ajor ( )
{
	GK ( gk , 1 );
	OneDef ( def1 );
	//TwoDef ( def , wall1 );
	
	ERRTSetObstacles ( def2 , true , true , true , true );
	OwnRobot[def2].face(Vec2(ball.Position.X,ball.Position.Y));
	ERRTNavigate2Point ( def2 , CircleAroundPoint(Vec2(ball.Position.X,ball.Position.Y),NormalizeAngle(-20+AngleWith(ball.Position , Vec2(side*3025,0))),590) ,0 , 40,&VELOCITY_PROFILE_AROOM);	
	
	ERRTSetObstacles ( attack , true , true , true , true );
	OwnRobot[attack].face(Vec2(ball.Position.X,ball.Position.Y));
	ERRTNavigate2Point ( attack , CircleAroundPoint(Vec2(ball.Position.X,ball.Position.Y),NormalizeAngle(AngleWith(ball.Position , Vec2(side*3025,0))),580) ,0 , 40,&VELOCITY_PROFILE_AROOM);
	
	ERRTSetObstacles(passgir, true, true, true, true);
	OwnRobot[passgir].face(Vec2(-side*3025, 0));
	ERRTNavigate2Point(passgir, Vec2(ball.Position.X, -sgn(ball.Position.Y)*1500), 0, 70);
}