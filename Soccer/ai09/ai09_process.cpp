#include "ai09.h"

void ai09::Process ( WorldState * worldState , GameSetting * setting , char * commands )
{
	AIDebug.Clear();
	
	internalProcessData(worldState, setting);
	
	if ( 0 )//ball.seenState == CompletelyOut )
	{
		//tech_challenge();
		//HaltAll();
		//tech_cmu();
		tech_mexico();
	}
	else if ( worldState ->refereeState.State )
	{
		if ( lastReferee != worldState ->refereeState.State->get() )
		{
			timer.start();
			lastReferee = worldState ->refereeState.State->get();
			randomParam = ( ( float ) ( rand ( ) ) ) / RAND_MAX;
		}
		//cout << "				rnd: " << randomParam <<	"	t: " << timer.time() << endl;

		if ( worldState ->refereeState.State->get() == GameState::GAME_OFF )
		{
			Stop ( );
			/*OwnRobot[attack].target.Angle = 90;OwnRobot[attack].State.Angle + 160;
			 Navigate2Point ( attack , Vec2 ( 1500 , -1700 ) , 0 , 100 );
			 OwnRobot[rmf].target.Angle = 0;OwnRobot[rmf].State.Angle + 160;
			 Navigate2Point ( rmf , Vec2 ( ball.Position.X-300 , -sgn(ball.Position.Y)*1600 ) , 0 , 100 );
			//Navigate2Point(passgir, Vec2(OwnRobot[attack].State.Position.X,OwnRobot[attack].State.Position.Y-50), 0, 100);
			 OwnRobot[dmf].target.Angle = 0;OwnRobot[dmf].State.Angle + 160;
			 Navigate2Point ( dmf , Vec2 ( -2000 , 200 ) , 0 , 100 );*/
		}
		else if ( worldState ->refereeState.State->get() == GameState::GAME_ON )
		{
			//NormalPlay();
			NewNormalPlay();
		}
		else if ( worldState ->refereeState.State->ourKickoff ( ) )
		{
			//tech_mexico();
			//tech_challenge();
			//if ( randomParam < 0.5 )
				//kickoff_us_farar( worldState ->refereeState.State->canKickBall() );
			//kickoff_us_zamini( worldState ->refereeState.State->canKickBall() );
			//else
			kickoff_us_chip( worldState ->refereeState.State->canKickBall() );
			//kickoff_us_farar( worldState ->refereeState.State->canKickBall() );
		}
		else if ( ( worldState ->refereeState.State->ourDirectKick ( ) ) || ( worldState ->refereeState.State->ourIndirectKick ( ) ) )
		{
			throwin_chip_shoot();
			//strategy_maker(playBook->strategy(playBook->strategy_size()-1));
			//strategy_maker(playBook->strategy(7));
			//tech_challenge();
			/*if ( ( ball.Position.X * side < -2500 ) )
			{
				//if ( randomParam < 0.5 )
					strategy_maker(playBook->strategy(9));
				//	corner_simple_chip();
				//else
					//corner_switch_pass();
				//strategy_maker(playBook->strategy(playBook->strategy_size()-1-(int)(randomParam*7.0)));
			}
			else if ( ( ball.Position.X * side < 0 ) )
			{
				//if ( randomParam < 0.5 )
					//corner_simple_chip();
				//else
				//	corner_switch_pass();
				//strategy_maker(playBook->strategy(playBook->strategy_size()-1));
				strategy_maker(playBook->strategy(8));//playBook->strategy_size()-1-(int)(randomParam*9.0)));
				//corner_simple_pass(true);
			}
			else {
				throwin_chip_shoot();
			}*/

			/*if ( ( ball.Position.X * side > 0 ) )
			{
				throwin_chip_shoot();
			}
			else {
				//strategy_maker(playBook->strategy(7));
				corner_simple_pass(true);
			}
			/*else if ( ball.Position.X * side > 2000 )
			{
				//throwin_chip_shoot();
				corner_simple_pass(false);
			}*/
			/*else
			{
				throwin_chip_shoot();
				//corner_simple_chip();
				//corner_simple_pass(false);
			}*/
			//strategy_maker(playBook->strategy(playBook->strategy_size()-1));
			//throwin_chip_shoot();
			//throwin_tu_omgh();
			//corner_simple_chip();
			//throwin_tu_omgh();
		}
		
		/*else if ( worldState ->refereeState.State->ourIndirectKick ( ) )
		 {
		 if ( ( ball.Position.X * side < -2500 ) )
		 {
		 //corner_simple_chip();
		 corner_switch_pass();
		 }
		 else if ( ( ball.Position.X * side < 0 ) )
		 {
		 //corner_simple_pass();
		 corner_switch_pass();
		 }
		 else
		 {
		 throwin_chip_shoot();
		 //corner_simple_pass();
		 }
		 }*/
		
		else if ( worldState ->refereeState.State->ourPenaltyKick ( ) )
		{
			/*if ( worldState ->refereeState.State->canKickBall() )
				NormalPlay();
			else
				Stop ( gk , def , dmf , lmf , rmf , attack );*/
			penalty_us_ghuz(worldState ->refereeState.State->canKickBall());
		}
		
		else if ( worldState ->refereeState.State->theirFreeKick() )
		{
			/*if ( ball.Position.X * side < 0 )
			{
				//corner_their_marker_ajor();
				//corner_their_khafan();
				corner_their_marker_karkas();
			}
			else*/
			{
				//throwin_their_khafan();
				//corner_their_khafan();
				//corner_their_marker_karkas();
				corner_their_global();
			}
		}
		
		else if ( worldState ->refereeState.State->theirKickoff() )
		{
			kickoff_their_one_wall();
			//kickoff_their_back_def();
		}
		
		else if ( worldState ->refereeState.State->theirPenaltyKick() )
		{
			penalty_their_simple();
			//penalty_their_gool(worldState->refereeState.State->canKickBall());
		}
		else if( worldState ->refereeState.State->get() == GameState::HALTED )
		{
			HaltAll();
		}
		else
		{
			Stop ( );
		}
	}
	else
	{
		//Stop ( );
		HaltAll();
		//OwnRobot[4].sendPID ( 0 , 0 , 500 , 1425 );
		//tech_mexico();
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