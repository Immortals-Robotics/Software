#include "../ai09.h"

float tetta = 0.0f;

void ai09::tech_mexico ( void )
{
	float hhh = sinDeg(tetta);
	float ggg = cosDeg(tetta);

	if ( ggg < 0 )
		ggg = -1;
	else
		ggg = 1;

	OwnRobot[attack].target.Angle = 90;
	Navigate2Point(attack, Vec2(1500, 1500*ggg));

	tetta +=0.1f;

	if ( tetta >= 360 )
		tetta = 0;
	//tetta = 135;
}