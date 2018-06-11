#include "../ai09.h"

Timer activeShootTimer_omid;

void ai09::NewNormalPlay ( void )
{
	GKHi(gk, 1, 0);
//	DefHi(def,NULL, 0);
	DefMid(def, lw, rw, NULL, false);
	
	CalcIsDefending();
	
	if (isDefending) {
		NormalPlayDef();
	}
	else {
		NormalPlayAtt();
	}

}
