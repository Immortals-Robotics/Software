#include "ai09.h"

void ai09::corner_their_def_karkas ( void )
{	
	GK ( gk , 1 );
	TwoDef(def1, def2);
	
	ERRTSetObstacles ( passgir , true , true , true , true );
	OwnRobot[passgir].face ( Vec2 ( -side*2995,0 ) );
	ERRTNavigate2Point ( passgir , PointOnConnectingLine ( Vec2 ( side*2995 , 0 ) , ball.Position , 1350 ) );
	
	DefenceWall(attack, 10);	
}