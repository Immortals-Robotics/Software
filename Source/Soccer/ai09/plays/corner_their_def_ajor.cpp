#include "../ai09.h"

void ai09::corner_their_def_ajor ( void )
{
	GK ( gk , 2 );
	TwoDef(def, dmf);
	
	Halt(lmf);
	
	ERRTSetObstacles ( rmf , true , true , true , false );
	ERRTNavigate2Point ( rmf , Vec2 ( side * 2350 , 0 ) );
	
	DefenceWall(attack);
}