#include "ai09.h"

void ai09::NewNormalPlay ( void )
{
	GK_Ghuz(gk, 1, 0);
	DefGhuz(def,NULL, 0);
	
	CalcIsDefending();
	
	if (isDefending) {
		NormalPlayDef();
	}
	else {
		NormalPlayAtt();
	}

}