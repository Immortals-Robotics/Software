#include "ai09.h"

void ai09::internalFinalize ( WorldState * worldState , GameSetting * setting , char * commands )
{
	for ( int i = 0 ; i < 6 ; i ++ )
		OwnRobot[i].makeSendingDataReady ( );
	
	for ( int i = 0 ; i < 6 ; i ++ )
	{
		for ( int j = 0 ; j < 8 ; j ++ )
		{
			commands[8*i+j] = OwnRobot[i].data[j];
		}
		//OwnRobot[i].data[9]=200;
		OwnRobot[i].halted = false;
	}
	
	for ( int i = 0 ; i < 12 ; i ++ )
	{
		for ( int j = 0 ; j < 10 ; j ++ )
		{
			worldState -> lastCMDS[i][j] = Vec3 ( 0.0f );
		}
	}
	
	for ( int i = 0 ; i < 6 ; i ++ )
	{
		if ( OwnRobot[i].State.seenState == CompletelyOut )
			continue;
		for ( int j = 0 ; j < 11 ; j ++ )
		{
			worldState -> lastCMDS[OwnRobot[i].State.vision_id][j] = OwnRobot[i].lastCMDs[j];
		}
	}
}