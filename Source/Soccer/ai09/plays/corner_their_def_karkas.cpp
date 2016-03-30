#include "../ai09.h"

void ai09::corner_their_def_karkas ( void )
{	
	GK ( gk , 1 );
	TwoDef(def, dmf);
	
	Halt(lmf);
	
	ERRTSetObstacles ( rmf , true , true , true , true );
	OwnRobot[rmf].face ( Vec2 ( -side*2995,0 ) );
	ERRTNavigate2Point ( rmf , PointOnConnectingLine ( Vec2 ( side*2995 , 0 ) , ball.Position , 1350 ) );
	
	DefenceWall(attack);	
}