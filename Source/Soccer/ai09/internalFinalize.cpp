#include <fstream>
#include "ai09.h"

void ai09::internalFinalize ( WorldState * worldState , GameSetting * setting )
{
	//bool saveKinoData = !OwnRobot[cmf].halted;
	
	for ( int i = 0 ; i < MAX_TEAM_ROBOTS ; i ++ )
		OwnRobot[i].makeSendingDataReady ( );


	for ( int i = 0 ; i < MAX_TEAM_ROBOTS ; i ++ )
	{
		senderBase->getCommand(&OwnRobot[i]);
		OwnRobot[i].halted = false;
	}
    senderBase->append_demo_data();

	
	for ( int i = 0 ; i < 12 ; i ++ )
	{
		for ( int j = 0 ; j < 10 ; j ++ )
		{
			worldState -> lastCMDS[i][j] = Vec3 ( 0.0f );
		}
	}
	
	for ( int i = 0 ; i < 8 ; i ++ )
	{
		if ( OwnRobot[i].State.seenState == CompletelyOut )
			continue;
		for ( int j = 0 ; j < 11 ; j ++ )//kheyli tof malie...
		{
			worldState -> lastCMDS[OwnRobot[i].State.vision_id][j] = OwnRobot[i].lastCMDs[j];
		}
	}
}