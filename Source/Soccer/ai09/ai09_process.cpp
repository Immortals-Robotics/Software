#include "ai09.h"

void ai09::Process ( WorldState * worldState , GameSetting * setting , char * commands )
{
	AIDebug.Clear();
	debugDraw = false;
	
	internalProcessData(worldState, setting);
	
	debugDraw = true;
	AddDebugCircle(ball.Position,40,Red);
	debugDraw = false;
		
	if ( 0 ) {
		currentPlay = "tech_cmu";
	}
	else if ( REF_playState )
	{
		if ( lastReferee != REF_playState->get() )
		{
			timer.start();
			lastReferee = REF_playState->get();
			randomParam = random.get();
			target_str = strategy_weight();
		}
		
		if ( REF_playState->get() == GameState::GAME_OFF )
		{
			oppRestarted = false;
			if (side*ball.Position.X>3000) {
				currentPlay = "Stop_def";
			}
			else {
				currentPlay = "Stop";
			}
		}
		else if ( REF_playState->get() == GameState::GAME_ON )
		{
			// Nik Uncomment
			
			//currentPlay = "strategy_maker";
			//currentPlayParam = playBook->strategy_size()-1;
			
			//   
			
			currentPlay = "NewNormalPlay";
			//currentPlay = "tech_mexico";
			//currentPlay = "tech_motion_ann";
			//if ( timer.time() > 30.0 || ball.seenState == CompletelyOut )
			//	currentPlay = "sharifcup_post_play";
            cout<<"IT's NORMALLLLLLLL"<<endl;
		}
		else if ( REF_playState->ourKickoff ( ) )
		{
			currentPlay = "kickoff_us_chip";
			//currentPlay = "kickoff_us_farar";
			currentPlay = "kickoff_us_pass";
			
			currentPlayParam = worldState ->refereeState -> State->canKickBall();
		}
		else if ( ( REF_playState->ourDirectKick ( ) ) || ( worldState ->refereeState -> State->ourIndirectKick ( ) ) )
		{
            if(1)
            {
                //currentPlay="corner_simple_pass";
                currentPlay="kickoff_us_zamini";

            }
            else {
                if (target_str != -1) {
                    currentPlay = "strategy_maker";
                    currentPlayParam = target_str;
                } else {
                    //if (side*ball.Position.X<800)
                    //    currentPlay = "throwin_us_outgir";
                    //else
                    currentPlay = "throwin_chip_shoot";
                }
            }
		}
		
		
		else if ( REF_playState->ourPenaltyKick ( ) )
		{
			currentPlay = "penalty_us_ghuz";
			currentPlayParam = worldState ->refereeState -> State->canKickBall();
		}
		
		else if ( REF_playState->theirFreeKick() )
		{
			//currentPlay = "corner_their_mrl";
			currentPlay = "corner_their_global";
			//currentPlay = "Stop";
		}
		
		else if ( REF_playState->theirKickoff() )
		{
			currentPlay = "kickoff_their_one_wall";
		}
		
		else if ( REF_playState->theirPenaltyKick() )
		{
			currentPlay = "penalty_their_simple";
		}
		else if( REF_playState->get() == GameState::HALTED )
		{
			currentPlay = "HaltAll";
		}
		else
		{
			currentPlay = "Stop";
		}
		
		if ( REF_playState->theirRestart() )
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
	else {
        HaltAll();
        cout<<"Oh NO!!!"<<endl;
    }
	
	
	for ( int i = 0 ; i < 6 ; i ++ )
	{
		if ( ( OwnRobot[i].State.seenState == CompletelyOut ) || ( !navigated[i] ) )
		{
			Halt ( i );
		}
	}

	internalFinalize(worldState, setting, commands);
}