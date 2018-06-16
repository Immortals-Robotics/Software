#include "../ai09.h"

Timer activeShootTimer_omid;

void ai09::NewNormalPlay ( void )
{
	GKHi(gk, 1, 0);
//	DefHi(def,NULL, 0);
	DefMid(def, rw, lw, NULL, false);
	
	CalcIsDefending();
	
	if (isDefending) {
		NormalPlayDef();
	}
	else {
		NormalPlayAtt();
	}

}
