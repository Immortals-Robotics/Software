#include "ai09.h"

void ai09::CalcIsDefending ( void )
{
	if (isDefending) {
		if (side*ball.Position.X<-200) {
			isDefending = false;
		}
	}
	else if (side*ball.Position.X>300)
	{
		isDefending = true;
	}
	
	if (timer.time()<2.0) {
		if (oppRestarted) {
			isDefending = true;
		}
		else {
			isDefending = false;
		}

	}
    
    //isDefending = false;
}