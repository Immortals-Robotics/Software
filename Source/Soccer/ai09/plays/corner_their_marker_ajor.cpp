#include "../ai09.h"

void ai09::corner_their_marker_ajor ( void )
{
	GK ( gk , 1 );
	OneDef ( def );
	
	Halt(lmf);
	
	ERRTSetObstacles ( dmf , true , true , true , false );
	ERRTNavigate2Point ( dmf , Vec2 ( side * 2350 , 0 ) );
	
	int index = findCruncherOpp(-1);
	if ( index == -1 )
	{
		ERRTSetObstacles ( rmf , true , true , true , false );
		OwnRobot[rmf].face(Vec2(ball.Position.X,ball.Position.Y));
		ERRTNavigate2Point ( rmf , CircleAroundPoint(Vec2(ball.Position.X,ball.Position.Y),NormalizeAngle(20+AngleWith(ball.Position , Vec2(side*2995,0))),590) ,0 , 100);
	}
	else
	{
		ERRTSetObstacles ( rmf , 1 );
		ERRTNavigate2Point ( rmf , PointOnConnectingLine ( OppRobot[index].Position , Vec2 ( side*2995 , 0 ) , 220.0f ) );
	}
	
	DefenceWall(attack);
}