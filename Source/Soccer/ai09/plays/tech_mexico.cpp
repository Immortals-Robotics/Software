#include "../ai09.h"

void ai09::tech_mexico ( void )
{
	float tetta = timer.time() * 0.5f;

	float hhh = sin(tetta);
	float ggg = cos(tetta);

	if ( ggg < 0 )
		ggg = -1;
	else
		ggg = 1;

	if ( hhh < 0 )
		hhh = -1;
	else
		hhh = 1;

	OwnRobot[attack].target.Angle = 0;
	Navigate2Point(attack, Vec2(2500 + 1700*hhh, 2700*ggg));

	tetta +=0.1f;

	if ( tetta >= 360 )
		tetta = 0;
	//tetta = 135;
}