#include "ai09.h"

void ai09::Process ( WorldState * worldState , GameSetting * setting )
{
    static int PRCS_CNT = 0;
	AIDebug.Clear();
    AIDebug.set_frame_id(PRCS_CNT++);

	debugDraw = false;
	
	internalProcessData(worldState, setting);
	
	debugDraw = true;
	AddDebugCircle(ball.Position,40,Red);
//	AddDebugLine(ball.Position,Vec2(ball.velocity.x,ball.velocity.y) + ball.Position, Black);

//    TVec2 spd_v= worldState->ball.path_dir;
//    AddDebugLine(ball.Position,spd_v + ball.Position, Yellow);
//    AddDebugLine(ball.Position,Vec2(ball.velocity.x,ball.velocity.y) + ball.Position, Yellow);
	debugDraw = false;


	if ( REF_playState )
	{
		if ( lastReferee != REF_playState->get() )
		{
			timer.start();
			lastReferee = REF_playState->get();
			randomParam = random.get();
			target_str = strategy_weight();
			FUNC_state = 0;
			FUNC_CNT = 0;
		}
		
		if ( REF_playState->get() == GameState::GAME_OFF )
		{
			oppRestarted = false;
			if (side * ball.Position.X > field_width * 0.7f ) {
				currentPlay = "Stop_def";
			}
			else {
				currentPlay = "Stop";
			}
		}
		else if ( REF_playState->get() == GameState::GAME_ON )
		{
			currentPlay = "NewNormalPlay";
//			currentPlay = "penalty_our_Shoot_Out";
		}
		else if ( REF_playState->ourKickoff ( ) )
		{
			currentPlay = "kickoff_us_chip";
			//currentPlay = "kickoff_us_farar";
			//currentPlay = "kickoff_us_pass";
			
			currentPlayParam = static_cast<uint32_t>(worldState ->refereeState -> State->canKickBall());
		}
		else if ( ( REF_playState->ourDirectKick ( ) ) || ( REF_playState->ourIndirectKick ( ) ) )
		{
            if (target_str != -1) {
                currentPlay = "strategy_maker";
                currentPlayParam = static_cast<uint32_t>(target_str);

            } else {
//                if (side*ball.Position.X<800)
//                    currentPlay = "throwin_us_outgir";
//                else
                currentPlay = "throwin_chip_shoot";
            }
			std::cout<<currentPlay<< std::endl;
		}
		else if ( REF_playState->ourPenaltyKick ( ) )
		{
			currentPlay = "penalty_us_ghuz";
			currentPlayParam = static_cast<uint32_t>(worldState ->refereeState -> State->canKickBall());
//			std::cout << "IN_PENALTY..."<<worldState ->refereeState -> State->canKickBall()<<std::endl;
		}
        else if(REF_playState->ourPlaceBall())
        {
//            targetBallPlacement->X = -2500;
//            targetBallPlacement->Y = -1500;
            currentPlay = "our_place_ball_shoot";
            currentPlay = "our_place_ball_shoot_V2";//COMMENT this if it's not working...
//			currentPlay = "our_place_ball_shoot_taki";
        }
		else if ( REF_playState->theirFreeKick() )
		{
			//currentPlay = "corner_their_mrl";
			currentPlay = "corner_their_global";
		}
		else if ( REF_playState->theirKickoff() )
		{
			currentPlay = "kickoff_their_one_wall";
		}
		else if ( REF_playState->theirPenaltyKick() )
		{
			currentPlay = "penalty_their_simple";
		}
        else if(REF_playState->theirPlaceBall())
        {
            currentPlay = "their_place_ball";
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
//	currentPlay = "my_test"; // TODO comment this in the game
//	currentPlay = "penalty_our_Shoot_Out";
//	currentPlay = "NewNormalPlay";
	
	if ( AIPlayBook.find(currentPlay) != AIPlayBook.end() )
		(this->*AIPlayBook[currentPlay])();
	else {
        HaltAll();
        //std::cout<<"Oh NO!!!"<<std::endl;
    }
	
	
	for ( int i = 0 ; i < 8 ; i ++ )
	{
		if ( ( OwnRobot[i].State.seenState == CompletelyOut ) || ( !navigated[i] ) )
		{
			Halt ( i );
		}
	}

	internalFinalize(worldState, setting);
}