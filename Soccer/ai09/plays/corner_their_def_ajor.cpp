#include "ai09.h"

void ai09::corner_their_def_ajor ( void )
{
	GK ( gk , 1 );
	TwoDef(def1, def2);
	
	ERRTSetObstacles ( passgir , true , true , true , false );
	ERRTNavigate2Point ( passgir , Vec2 ( side * 2350 , 0 ) );
	
	DefenceWall(attack, 10);
}