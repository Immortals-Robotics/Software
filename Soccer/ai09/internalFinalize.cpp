#include <fstream>
#include "ai09.h"

void ai09::internalFinalize ( WorldState * worldState , GameSetting * setting , char * commands )
{
	//bool saveKinoData = !OwnRobot[cmf].halted;
	
	for ( int i = 0 ; i < 6 ; i ++ )
		OwnRobot[i].makeSendingDataReady ( );
	
	for ( int i = 0 ; i < 6 ; i ++ )
	{
		for ( int j = 0 ; j < 11 ; j ++ )
		{
			commands[11*i+j] = OwnRobot[i].data[j];
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
	
	/*if ( saveKinoData )
	{
		ofstream kinoData ( "kino_data.txt" , ios::ate|ios::out|ios::app );
	
	kinoData << OwnRobot[cmf].State.velocity.x << " " << OwnRobot[cmf].State.velocity.y << " " << OwnRobot[cmf].State.Angle << "	";
	kinoData << OwnRobot[cmf].State.Position.X << " " << OwnRobot[cmf].State.Position.Y << " " << OwnRobot[cmf].State.Angle << "	";
	kinoData << OwnRobot[cmf].lastCMDs[0].X << " " << OwnRobot[cmf].lastCMDs[0].Y << " " << OwnRobot[cmf].target.Angle << endl;
	
	kinoData.close();
	}*/
	
}