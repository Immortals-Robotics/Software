#include "ai09.h"

void ai09::internalProcessData ( WorldState * worldState , GameSetting * setting )
{
    this -> oppGK = worldState -> oppGK;
    
	this->ball = worldState -> ball;
	if ( ball.seenState != CompletelyOut )
		this->ballHist.push_back(this->ball);
	if ( this->ballHist.size() > maxBallHist )
		this->ballHist.pop_front();
	debugDraw = true;
	CalculateBallTrajectory();
	debugDraw = false;
	
	for ( int i = 0 ; i < 6 ; i ++ )
	{
		this->OwnRobot[i].State = worldState -> OwnRobot[OwnRobot[i].vision_id];
		
		if ( !worldState ->refereeState.State || worldState ->refereeState.State->get() == GameState::GAME_OFF )
		{
			if ( OwnRobot[i].State.OutForSubsitute )
			{
				for ( int j = 0 ; j < MAX_ROBOTS ; j ++ )
				{
					if ( ( worldState -> OwnRobot[j].seenState == Seen ) && ( fabs(worldState->OwnRobot[j].Position.X)<field_width ) && ( fabs(worldState->OwnRobot[j].Position.Y)<field_height ) )
					{
						bool suitable = true;
						for ( int k = 0 ; k < 6 ; k ++ )
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
		//cout << OwnRobot[i].vision_id << "	";
		
		this->OwnRobot[i].set_serial_id(VisionSerialTrans[OwnRobot[i].vision_id]);
		//this->OwnRobot[i].oldRobot = true;
		//if ( ( i != gk ) && ( i != def1 ) )
		this->OwnRobot[i].oldRobot = false;
		
		this->navigated[i] = false;
		//if ((OwnRobot[i].vision_id==7)||(OwnRobot[i].vision_id==4)||(OwnRobot[i].vision_id==0)) {
		//	this->OwnRobot[i].oldRobot = true;
		//}
		//if (OwnRobot[i].vision_id==10) {
		//	this->OwnRobot[i].oldRobot = true;
		//}
		//this->OwnRobot[def].oldRobot = true;
		//if(i==passgir)
		//	this->OwnRobot[i].oldRobot = true;
		
	}
	//cout << endl;
	
	for ( int i = 0 ; i < 12 ; i ++ )
		this->OppRobot[i] = worldState -> OppRobot[i];
	
	this->OwnRobotNum = worldState -> ownRobots_num;
	this->OppRobotNum = worldState -> oppRobots_num;
	
	if ( setting -> side == Right )
		this->side = 1;
	else
		this->side = -1;
}