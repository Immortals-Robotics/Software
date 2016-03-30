#include "../ai09.h"

void ai09::stop_ajor ( )
{
	GK ( gk , 1 );
	OneDef ( def );
	//TwoDef ( def , wall1 );
	
	ERRTSetObstacles ( dmf , true , true , true , true );
	OwnRobot[dmf].face(Vec2(ball.Position.X,ball.Position.Y));
	ERRTNavigate2Point ( dmf , CircleAroundPoint(Vec2(ball.Position.X,ball.Position.Y),NormalizeAngle(-20+AngleWith(ball.Position , Vec2(side*3025,0))),590) ,0 , 40,&VELOCITY_PROFILE_AROOM);	
	
	Halt(lmf);
	
	ERRTSetObstacles ( attack , true , true , true , true );
	OwnRobot[attack].face(Vec2(ball.Position.X,ball.Position.Y));
	ERRTNavigate2Point ( attack , CircleAroundPoint(Vec2(ball.Position.X,ball.Position.Y),NormalizeAngle(AngleWith(ball.Position , Vec2(side*3025,0))),580) ,0 , 40,&VELOCITY_PROFILE_AROOM);
	
	ERRTSetObstacles(rmf, true, true, true, true);
	OwnRobot[rmf].face(Vec2(-side*3025, 0));
	ERRTNavigate2Point(rmf, Vec2(ball.Position.X, -sgn(ball.Position.Y)*1500), 0, 70);
}