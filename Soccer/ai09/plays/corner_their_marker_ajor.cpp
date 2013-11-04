#include "ai09.h"

void ai09::corner_their_marker_ajor ( void )
{
	GK ( gk , 1 );
	OneDef ( def1 );
	
	ERRTSetObstacles ( def2 , true , true , true , false );
	ERRTNavigate2Point ( def2 , Vec2 ( side * 2350 , 0 ) );
	
	int index = findCruncherOpp(-1);
	if ( index == -1 )
	{
		ERRTSetObstacles ( passgir , true , true , true , false );
		OwnRobot[passgir].face(Vec2(ball.Position.X,ball.Position.Y));
		ERRTNavigate2Point ( passgir , CircleAroundPoint(Vec2(ball.Position.X,ball.Position.Y),NormalizeAngle(20+AngleWith(ball.Position , Vec2(side*2995,0))),590) ,0 , 100);
	}
	else
	{
		ERRTSetObstacles ( passgir , 1 );
		ERRTNavigate2Point ( passgir , PointOnConnectingLine ( OppRobot[index].Position , Vec2 ( side*2995 , 0 ) , 220.0f ) );
	}
	
	DefenceWall(attack, 10);
}