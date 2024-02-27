#include "../ai09.h"

void ai09::corner_their_marker_karkas ( void )
{
	/*GK ( gk , 1 );
	OneDef ( def );*/
	
	GKHi(gk);
	DefHi(def);
	
	Halt(lmf);
	
	int index = findCruncherOpp(-1);
	if ( index == -1 )
	{
		ERRTSetObstacles ( dmf , true , true , true , false );
		OwnRobot[dmf].face(Vec2(ball.Position.X,ball.Position.Y));
		ERRTNavigate2Point ( dmf , CircleAroundPoint(Vec2(ball.Position.X,ball.Position.Y),NormalizeAngle(-20+AngleWith(ball.Position , Vec2(side*2995,0))),590) ,0 , 100);
	}
	else
	{
		ERRTSetObstacles ( dmf , 1 );
		ERRTNavigate2Point ( dmf, PointOnConnectingLine ( OppRobot[index].Position , Vec2 ( side*2995 , 0 ) , 220.0f ) );
	}
	
	ERRTSetObstacles ( rmf , true , true , true , true );
	OwnRobot[rmf].face ( Vec2 ( -side*2995,0 ) );
	ERRTNavigate2Point ( rmf , PointOnConnectingLine ( Vec2 ( side*2995 , 0 ) , ball.Position , 1350 ) );

	DefenceWall(attack);
}