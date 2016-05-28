#include "ai09.h"

void ai09::Process ( WorldState * worldState , GameSetting * setting , char * commands )
{
	AIDebug.Clear();
	debugDraw = false;
	
	internalProcessData(worldState, setting);
	
	debugDraw = true;
	AddDebugCircle(ball.Position,40,Red);
	debugDraw = false;
		
	if ( 0 )
		currentPlay = "tech_cmu";
	
	else if ( worldState ->refereeState.State )
	{
		if ( lastReferee != worldState ->refereeState.State->get() )
		{
			timer.start();
			lastReferee = worldState ->refereeState.State->get();
			randomParam = random.get();
			target_str = strategy_weight();
		}
		
		if ( worldState ->refereeState.State->get() == GameState::GAME_OFF )
		{
			oppRestarted = false;
			if (side*ball.Position.X>3000) {
				currentPlay = "Stop_def";
			}
			else {
				currentPlay = "Stop";
			}
		}
		else if ( worldState ->refereeState.State->get() == GameState::GAME_ON )
		{
			// Nik Uncomment
			
			//currentPlay = "strategy_maker";
			//currentPlayParam = playBook->strategy_size()-1;
			
			//   
			
			//currentPlay = "NewNormalPlay";
			currentPlay = "tech_mexico";
			//currentPlay = "tech_motion_ann";
			//if ( timer.time() > 30.0 || ball.seenState == CompletelyOut )
			//	currentPlay = "sharifcup_post_play";
		}
		else if ( worldState ->refereeState.State->ourKickoff ( ) )
		{
			currentPlay = "kickoff_us_chip";
			//currentPlay = "kickoff_us_farar";
			//currentPlay = "kickoff_us_pass";
			
			currentPlayParam = worldState ->refereeState.State->canKickBall();
		}
		else if ( ( worldState ->refereeState.State->ourDirectKick ( ) ) || ( worldState ->refereeState.State->ourIndirectKick ( ) ) )
		{
			if (target_str!=-1) {
				currentPlay = "strategy_maker";
				currentPlayParam = target_str;
			}
			else {
                //if (side*ball.Position.X<800)
                //    currentPlay = "throwin_us_outgir";
                //else
                    currentPlay = "throwin_chip_shoot";
			}
		}
		
		
		else if ( worldState ->refereeState.State->ourPenaltyKick ( ) )
		{
			currentPlay = "penalty_us_ghuz";
			currentPlayParam = worldState ->refereeState.State->canKickBall();
		}
		
		else if ( worldState ->refereeState.State->theirFreeKick() )
		{
			//currentPlay = "corner_their_mrl";
			currentPlay = "corner_their_global";
			//currentPlay = "Stop";
		}
		
		else if ( worldState ->refereeState.State->theirKickoff() )
		{
			currentPlay = "kickoff_their_one_wall";
		}
		
		else if ( worldState ->refereeState.State->theirPenaltyKick() )
		{
			currentPlay = "penalty_their_simple";
		}
		else if( worldState ->refereeState.State->get() == GameState::HALTED )
		{
			currentPlay = "HaltAll";
		}
		else
		{
			currentPlay = "Stop";
		}
		
		if ( worldState->refereeState.State->theirRestart() )
		{
			oppRestarted = true;
		}
	}
	else
	{
        currentPlay = "Stop";
		//currentPlay = "HaltAll";
	}
	
	
	if ( AIPlayBook.find(currentPlay) != AIPlayBook.end() )
		(this->*AIPlayBook[currentPlay])();
	else
		HaltAll();
	
	
	for ( int i = 0 ; i < 6 ; i ++ )
	{
		if ( ( OwnRobot[i].State.seenState == CompletelyOut ) || ( !navigated[i] ) )
		{
			Halt ( i );
		}
	}
	
	internalFinalize(worldState, setting, commands);
}