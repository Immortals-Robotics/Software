#include "ai09.h"

void ai09::internalProcessData ( WorldState * worldState , GameSetting * setting )
{
    this -> oppGK = worldState -> refereeState -> oppGK;
    
	this->ball = worldState -> ball;
	if ( ball.seenState != CompletelyOut )
		this->ballHist.push_back(this->ball);
	if ( this->ballHist.size() > maxBallHist )
		this->ballHist.pop_front();
	//debugDraw = true;
	CalculateBallTrajectory();
	debugDraw = false;
	
	for ( int i = 0 ; i < Setting::kMaxOnFieldTeamRobots ; i ++ )
	{
		bool halt_this_robot_for_now = false;
		this->OwnRobot[i].State = worldState -> OwnRobot[OwnRobot[i].vision_id];
		
		if ( !worldState ->refereeState -> State || worldState ->refereeState -> State->get() == GameState::GAME_OFF )
		{
			if ( OwnRobot[i].State.OutForSubsitute )
			{
				for ( int j = 0 ; j < Setting::kMaxRobots ; j ++ )
				{
					if ( ( worldState -> OwnRobot[j].seenState == Seen ) && ( fabs(worldState->OwnRobot[j].Position.X)<field_width ) && ( fabs(worldState->OwnRobot[j].Position.Y)<field_height ) )
					{
						bool suitable = true;
						for ( int k = 0 ; k < Setting::kMaxOnFieldTeamRobots; k ++ )
						{
							if ( OwnRobot[k].vision_id == j )
							{
								suitable = false;
								break;
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
		//std::cout << OwnRobot[i].vision_id << "	";
		
		this->OwnRobot[i].set_serial_id(OwnRobot[i].vision_id);
		//this->OwnRobot[i].oldRobot = true;
		//if ( ( i != gk ) && ( i != def1 ) )
		//this->OwnRobot[i].oldRobot = false;

		this->OwnRobot[i].shoot = 0;
		this->OwnRobot[i].dribbler = 0;
		this->OwnRobot[i].shoot = 0;
		this->OwnRobot[i].chip = 0;
		this->OwnRobot[i].Break = 0;
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
	//std::cout << std::endl;
	
	for ( int i = 0 ; i < Setting::kMaxRobots ; i ++ )
		this->OppRobot[i] = worldState -> OppRobot[i];
	
	this->OwnRobotNum = worldState -> ownRobots_num;
	this->OppRobotNum = worldState -> oppRobots_num;
	
	if ( setting -> our_side == RIGHT_SIDE )
		this->side = 1;
	else
		this->side = -1;
}