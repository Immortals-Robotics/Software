#include "../ai09.h"

Timer activeShootTimer_omid;

void ai09::NewNormalPlay ( void )
{
//	GKHi(gk, 0);
	GKHi_Simple(gk,0);
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
