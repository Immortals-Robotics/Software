#include "../ai09.h"

void ai09::NewNormalPlay ( void )
{
	GKHi(gk, 1, 0);
	DefHi(def,NULL, 0);
	
	CalcIsDefending();
	
	if (isDefending) {
		NormalPlayDef();
	}
	else {
		NormalPlayAtt();
	}

}