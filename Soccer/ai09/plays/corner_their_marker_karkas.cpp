#include "ai09.h"

void ai09::corner_their_marker_karkas ( void )
{
	GK ( gk , 1 );
	OneDef ( def1 );
	
	int index = findCruncherOpp(-1);
	if ( index == -1 )
	{
		ERRTSetObstacles ( def2 , true , true , true , false );
		OwnRobot[def2].face(Vec2(ball.Position.X,ball.Position.Y));
		ERRTNavigate2Point ( def2 , CircleAroundPoint(Vec2(ball.Position.X,ball.Position.Y),NormalizeAngle(-20+AngleWith(ball.Position , Vec2(side*2995,0))),590) ,0 , 100);
	}
	else
	{
		ERRTSetObstacles ( def2 , 1 );
		ERRTNavigate2Point ( def2 , PointOnConnectingLine ( OppRobot[index].Position , Vec2 ( side*2995 , 0 ) , 220.0f ) );
	}
	
	ERRTSetObstacles ( passgir , true , true , true , true );
	OwnRobot[passgir].face ( Vec2 ( -side*2995,0 ) );
	ERRTNavigate2Point ( passgir , PointOnConnectingLine ( Vec2 ( side*2995 , 0 ) , ball.Position , 1350 ) );

	DefenceWall(attack, 10);
}