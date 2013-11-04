#include "ai09.h"

void ai09::CalcIsDefending ( void )
{
	if (isDefending) {
		if (side*ball.Position.X<-300) {
			isDefending = false;
		}
	}
	else if (side*ball.Position.X>300)
	{
		isDefending = true;
	}
}