#include "ai09.h"
#include <iostream>
using namespace std;

#include <vector>

void ai09::Stop ( void )
{
	debugDraw = true;
	AddDebugCircle(ball.Position,500,Red);
	debugDraw = false;
	
	if (timer.time()<0.5) {
		if ( side * OwnRobot[mid1].State.Position.Y < side * OwnRobot[mid2].State.Position.Y )
		{ 
			swap(mid1, mid2);
		}
	}
	GKHi( gk , 1 , true );
	DefHi(def,NULL, true);
	
	//GK (gk , 1 , true );
	//OneDef(def1,NULL, true);

	//TwoDef ( def1 , def2 , NULL );
	//TwoDef ( def , wall1 );
	
	ERRTSetObstacles ( dmf , true , true , true , true );
	OwnRobot[dmf].face(ball.Position);
	ERRTNavigate2Point ( dmf , PointOnConnectingLine(ball.Position, Vec2(side*3025, 0), DIS(ball.Position, Vec2(side*3025, 0))/3.0f) ,0 , 40,&VELOCITY_PROFILE_AROOM);
	

	ERRTSetObstacles ( lmf , true , true , true , true );
	OwnRobot[lmf].face(Vec2(ball.Position.X,ball.Position.Y));
	ERRTNavigate2Point ( lmf , CircleAroundPoint(Vec2(ball.Position.X,ball.Position.Y),NormalizeAngle(-20+AngleWith(ball.Position , Vec2(side*3025,0))),590) ,0 , 40,&VELOCITY_PROFILE_AROOM);	
	
	ERRTSetObstacles ( rmf , true , true , true , true );
	OwnRobot[rmf].face(Vec2(ball.Position.X,ball.Position.Y));
	ERRTNavigate2Point ( rmf , CircleAroundPoint(Vec2(ball.Position.X,ball.Position.Y),NormalizeAngle(20+AngleWith(ball.Position , Vec2(side*3025,0))),590) ,0 , 40,&VELOCITY_PROFILE_AROOM);

	ERRTSetObstacles ( cmf , true , true , true , true );
	OwnRobot[cmf].face(Vec2(ball.Position.X,ball.Position.Y));
	ERRTNavigate2Point ( cmf , CircleAroundPoint(Vec2(ball.Position.X,ball.Position.Y),NormalizeAngle(AngleWith(ball.Position , Vec2(side*3025,0))),590) ,0 , 40,&VELOCITY_PROFILE_AROOM);
	//side=-side;
}