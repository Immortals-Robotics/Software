#include "ai09.h"

#include "helpers.h"

float tetta = 0;

void ai09::tech_mexico ( void )
{
	int rn = 0;
	ERRTSetObstacles ( rn , true , false , true , true );
	OwnRobot[rn].target.Angle = 90.0f;
	ERRTNavigate2Point ( rn   , Vec2 (  -1000.0f , 1800.0f*sin ( ( tetta + 00 ) * 3.1415 / 180.0 ) ) , 0 , 100 );

	ERRTSetObstacles ( rn+1 , true , false , true , true );
	OwnRobot[rn+1].target.Angle = 90.0f;
	ERRTNavigate2Point ( rn+1 , Vec2 ( -500.0f , 1800.0f*sin ( ( tetta + 30 ) * 3.1415 / 180.0 ) ) , 0 , 100 );

	ERRTSetObstacles ( rn+2 , true , false , true , true );
	OwnRobot[rn+2].target.Angle = 90.0f;
	ERRTNavigate2Point ( rn+2 , Vec2 ( 0.0f , 1800.0f*sin ( ( tetta + 60 ) * 3.1415 / 180.0 ) ) , 0 , 100 );

	ERRTSetObstacles ( rn+3 , true , false , true , true );
	OwnRobot[rn+3].target.Angle = 90.0f;
	ERRTNavigate2Point ( rn+3 , Vec2 ( 500.0f , 1800.0f*sin ( ( tetta + 90 ) * 3.1415 / 180.0 ) ) , 0 , 100 );

	ERRTSetObstacles ( rn+4 , true , false , true , true );
	OwnRobot[rn+4].target.Angle = 90.0f;
	ERRTNavigate2Point ( rn+4 , Vec2 ( 1000.0f , 1800.0f*sin ( ( tetta + 120 ) * 3.1415 / 180.0 ) ) , 0 , 100 );

	tetta +=1.2;

	if ( tetta >= 360 )
		tetta = 0;
}