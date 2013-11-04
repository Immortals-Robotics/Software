#include "ai09.h"
#include <iostream>
#include "helpers.h"

int state = 0;
int tside = 1;

void ai09::tech_cmu ( void )
{
	if ( state == 0 )
	{
		state = 1;

		for ( int i = 0 ; i < 5 ; i ++ )
		{
			if ( dis ( OwnRobot[i].State.Position.X , OwnRobot[i].State.Position.Y , 1200.0f - 250.0f * i , tside*1200.0f ) > 20 )
				state = 0;
		}
	}
	else if ( state >= 6 )
	{
		//tside = -tside;
		//state = 0;
	}
	else
	{
		if ( dis ( OwnRobot[state-1].State.Position.X , OwnRobot[state-1].State.Position.Y , 1200.0f - ( state - 1 ) * 250.0f , tside*1450.0f ) < 20 )
			state ++;
	}

	for ( int i = state ; i < 5 ; i ++ )
	{
		ERRTSetObstacles ( i , true , false , true , true );
		OwnRobot[i].target.Angle = tside*90.0f;
		ERRTNavigate2Point ( i   , Vec2 (  1200.0f - i * 250.0f , tside*1200.0f ) , 0 , 100 );
	}
	for ( int i = 0 ; i < min(5,state) ; i ++ )
	{
		ERRTSetObstacles ( i , true , false , true , true );
		OwnRobot[i].target.Angle = -tside*120.0f;
		ERRTNavigate2Point ( i   , Vec2 (  1200.0f - i * 250.0f , tside*1450.0f ) , 0 , 100 );
	}
	//std::cout<<state<<std::endl<<side<<std::endl;

}