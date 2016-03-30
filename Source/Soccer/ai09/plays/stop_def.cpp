#include "../ai09.h"

void ai09::Stop_def ( void )
{
	GKHi( gk , 1 , true );
	DefHi(def,NULL, true);
	
	map<int,TVec2> static_pos;
	static_pos[dmf] = Vec2(side*2300, -sgn(ball.Position.Y)*800);
	static_pos[mid1] = Vec2(side*2000, 300);
	static_pos[mid2] = Vec2(side*2000, 300);
	
	ERRTSetObstacles ( dmf , true , true , true , true );
	OwnRobot[dmf].face(ball.Position);
	ERRTNavigate2Point ( dmf , static_pos[dmf] ,0 , 40,&VELOCITY_PROFILE_AROOM);
	
	
	ERRTSetObstacles ( mid1 , true , true , true , true );
	OwnRobot[mid1].face(Vec2(ball.Position.X,ball.Position.Y));
	ERRTNavigate2Point ( mid1 , static_pos[mid1] ,0 , 40,&VELOCITY_PROFILE_AROOM);	
	
	ERRTSetObstacles ( mid2 , true , true , true , true );
	OwnRobot[mid2].face(Vec2(ball.Position.X,ball.Position.Y));
	ERRTNavigate2Point ( mid2 , static_pos[mid2] ,0 , 40,&VELOCITY_PROFILE_AROOM);	
	
	ERRTSetObstacles ( attack , true , true , true , true );
	OwnRobot[attack].face(Vec2(ball.Position.X,ball.Position.Y));
	ERRTNavigate2Point ( attack , CircleAroundPoint(Vec2(ball.Position.X,ball.Position.Y),NormalizeAngle(AngleWith(ball.Position , Vec2(side*field_width,0))),580) ,0 , 40,&VELOCITY_PROFILE_AROOM);
	//side=-side;
}