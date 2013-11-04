#include "ai09.h"

void ai09::internalProcessData ( WorldState * worldState , GameSetting * setting )
{
	this->ball = worldState -> ball;
	
	for ( int i = 0 ; i < 5 ; i ++ )
	{
		//if ( i == 3 )	continue;
		this->OwnRobot[i].State = worldState -> OwnRobot[OwnRobot[i].vision_id];
		
		if ( !worldState ->refereeState.State || worldState ->refereeState.State->get() == GameState::GAME_OFF )
		{
			if ( OwnRobot[i].State.seenState == CompletelyOut )
			{
				for ( int j = 0 ; j < MAX_ROBOTS ; j ++ )
				{
					if ( worldState -> OwnRobot[j].seenState == Seen )
					{
						bool suitable = true;
						for ( int k = 0 ; k < 5 ; k ++ )
						{
							if ( OwnRobot[k].vision_id == j )
							{
								if ( 0 )//i < k ) 
								{
									OwnRobot[k].set_vision_id(OwnRobot[i].vision_id);
									break;
								}
								else {
									suitable = false;
									break;
								}
							}
						}
						if ( suitable )
						{
							OwnRobot[i].set_vision_id(j);
							break;
						}
					}
				}
			}
		}
		cout << OwnRobot[i].vision_id << "	";
		
		this->OwnRobot[i].set_serial_id(VisionSerialTrans[OwnRobot[i].vision_id]);
		//if ( ( i != gk ) && ( i != def1 ) )
		this->OwnRobot[i].oldRobot = false;
		this->OwnRobot[gk].oldRobot = true;
		//this->OwnRobot[def1].oldRobot = true;
		
	}
	cout << endl;
	
	for ( int i = 0 ; i < 12 ; i ++ )
		this->OppRobot[i] = worldState -> OppRobot[i];
	
	this->OwnRobotNum = worldState -> ownRobots_num;
	this->OppRobotNum = worldState -> oppRobots_num;
	
	if ( setting -> side == Right )
		this->side = 1;
	else
		this->side = -1;
}