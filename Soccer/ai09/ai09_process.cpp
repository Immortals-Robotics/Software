#include "ai09.h"

void ai09::Process ( WorldState * worldState , GameSetting * setting , char * commands )
{
	AIDebug.Clear();
	
	internalProcessData(worldState, setting);
	
	//cout << calculateOpenAngleToGoal ( OwnRobot[passgir].State.Position , passgir ).X << endl;
	//TVec2 anssss = calculateOpenAngleToGoal ( Vec2(-2000,-2000) , 100 );
	//cout << anssss.X << "	" << anssss.Y << endl;
	
	if ( 0 )//ball.seenState == CompletelyOut )
	{
		//tech_challenge();
		//HaltAll();
		//calculateOneTouchAngle(passgir,ball.Position);
		tech_cmu();
	}
	else if ( worldState ->refereeState.State )
	{
		if ( lastReferee != worldState ->refereeState.State->get() )
		{
			timer.start();
			lastReferee = worldState ->refereeState.State->get();
			randomParam = ( ( float ) ( rand ( ) ) ) / RAND_MAX;
		}
		if ( worldState ->refereeState.State->get() == GameState::GAME_OFF )
		{
			Stop ( gk , def1 , def2 , passgir , attack );
			/*OwnRobot[attack].target.Angle = 90;OwnRobot[attack].State.Angle + 160;
			 Navigate2Point ( attack , Vec2 ( -2000 , -1000 ) , 0 , 100 );
			 OwnRobot[passgir].target.Angle = 0;OwnRobot[passgir].State.Angle + 160;
			 Navigate2Point ( passgir , Vec2 ( -2000 , 400 ) , 0 , 100 );
			 OwnRobot[def2].target.Angle = 0;OwnRobot[def2].State.Angle + 160;
			 Navigate2Point ( def2 , Vec2 ( -2000 , -400 ) , 0 , 100 );*/
			//tech_challenge();
		}
		else if ( worldState ->refereeState.State->get() == GameState::GAME_ON )
		{
			if ( ball.Position.X * side > 0 )
				this->isDefending = true;
			else
				this->isDefending = false;
			
			if ( 0 )
			{
				OwnRobot[attack].target.Angle = 90;OwnRobot[attack].State.Angle + 160;
				Navigate2Point ( attack , Vec2 ( -2000 , 1000 ) , 0 , 100 );
				OwnRobot[passgir].target.Angle = 180;OwnRobot[passgir].State.Angle + 160;
				Navigate2Point ( passgir , Vec2 ( 2000 , 400 ) , 0 , 100 );
				OwnRobot[def2].target.Angle = 180;OwnRobot[def2].State.Angle + 160;
				Navigate2Point ( def2 , Vec2 ( 2000 , -400 ) , 0 , 100 );
				//backPass(attack,AngleWith ( Vec2 ( -side * 3033 , 0 ) , ball.Position ) );
				//HaltAll();
			}
			else if ( ( ball.Position.X * side < -1500 ) && ( fabs ( ball.Position.Y ) > 1500 ) )
			{
				reached = true;
				GK ( gk , 2 );
				TwoDef ( def1 , def2 );
				//OneDef(def1);
				
				if (( ( DIS ( ball.Position , OwnRobot[attack].State.Position ) < 250 ) && ( OwnRobot[attack].State.velocity.magnitude < 100 ) )|| ( reached ) )
				{
					tech_circle(attack,AngleWith ( Vec2 ( -side*1000 , -sgn ( ball.Position.Y ) * 1000 ) , ball.Position ) ,2,0);
					reached = true;
				}
				else
					tech_circle(attack,AngleWith ( Vec2 ( -side*1000 , -sgn ( ball.Position.Y ) * 1000 ) , ball.Position ) ,0,0);
				
				if ( ( fabs ( NormalizeAngle ( ball.velocity.direction - AngleWith ( ball.Position , Vec2 ( OwnRobot[passgir].State.Position.X + BAR * cosDeg ( OwnRobot[passgir].State.Angle ) , OwnRobot[passgir].State.Position.Y + BAR * sinDeg ( OwnRobot[passgir].State.Angle ) ) ) ) ) < 75 ) && ( fabs(NormalizeAngle(180+ball.velocity.direction-OwnRobot[passgir].State.Angle)) < 100 ) && ( ball.velocity.magnitude > 50 ) )//&&(abs(ball.vel_angle-90)>0.01)&&(abs(ball.vel_angle+90)>0.01)&&(abs(ball.vel_angle-180)>0.01)&&(abs(ball.vel_angle+180)>0.01))
				{
					WaitForPass ( passgir );
					hys = 120;
				}
				else if (( hys > 0 )&& ( ball.velocity.magnitude > 50 ))// &&(abs(ball.vel_angle-90)>0.01)&&(abs(ball.vel_angle+90)>0.01)&&(abs(ball.vel_angle-180)>0.01)&&(abs(ball.vel_angle+180)>0.01))
				{
					WaitForPass ( passgir );
					hys --;
				}
				
				else
				{
					hys = 0;
					OwnRobot[passgir].face ( Vec2 ( -side*3033 , 0 ) );
					Navigate2Point ( passgir , Vec2 ( -side*1500 , -sgn(ball.Position.Y )* 1000 ) );
				}
			}
			else
			{
				reached = false;
				GK ( gk , 2 );
				TwoDef ( def1 , def2 );
				//OneDef(def1);
				OwnRobot[passgir].Shoot( 15 );
				if ( ( fabs ( NormalizeAngle ( ball.velocity.direction - AngleWith ( ball.Position , Vec2 ( OwnRobot[passgir].State.Position.X + BAR * cosDeg ( OwnRobot[passgir].State.Angle ) , OwnRobot[passgir].State.Position.Y + BAR * sinDeg ( OwnRobot[passgir].State.Angle ) ) ) ) ) < 65 ) && ( fabs(NormalizeAngle(180+ball.velocity.direction-OwnRobot[passgir].State.Angle)) < 150 ) && ( ball.velocity.magnitude > 700 ) )//&&(abs(ball.vel_angle-90)>0.01)&&(abs(ball.vel_angle+90)>0.01)&&(abs(ball.vel_angle-180)>0.01)&&(abs(ball.vel_angle+180)>0.01))
				{
					WaitForPass ( passgir );
					hys = 120;
				}
				else if (( hys > 0 )&& ( ball.velocity.magnitude > 50 ))// &&(abs(ball.vel_angle-90)>0.01)&&(abs(ball.vel_angle+90)>0.01)&&(abs(ball.vel_angle-180)>0.01)&&(abs(ball.vel_angle+180)>0.01))
				{
					WaitForPass ( passgir );
					hys --;
				}
				
				else
				{
					hys = 0;
					OwnRobot[passgir].face ( Vec2 ( -side*3033 , 0 ) );
					ERRTSetObstacles ( passgir );
					if ( !isDefending )
						ERRTNavigate2Point ( passgir , Vec2 ( -side*500 , -sgn(ball.Position.Y )* 1500 ) );
					else
					{
						int index = findCruncherOpp(-1);
						if ( index == -1 )
						{
							ERRTNavigate2Point ( passgir , Vec2 ( side*500,0));//CircleAroundPoint(Vec2(ball.Position.X,ball.Position.Y),NormalizeAngle(20+AngleWith(ball.Position , Vec2(side*2995,0))),590) ,0 , 100);
						}
						else
						{
							ERRTNavigate2Point ( passgir , PointOnConnectingLine ( OppRobot[index].Position , Vec2 ( side*2995 , 0 ) , 220.0f ) );
						}
					}
				}
				ERRTSetObstacles ( attack );
				//tech_circle(attack,AngleWith ( Vec2 ( -side * 3033 , 0 ) , ball.Position ) ,15);
				//tech_circle(attack, NormalizeAngle( 180+calculateOpenAngleToGoal(ball.Position, attack).X), 15 );
				Halt(attack);
			}
		}
		else if ( worldState ->refereeState.State->ourKickoff ( ) )
		{
			//if ( randomParam < 0.5 )
			//	kickoff_us_pass( worldState ->refereeState.State->canKickBall() );
			//else
				kickoff_us_chip( worldState ->refereeState.State->canKickBall() );
		}
		else if ( ( worldState ->refereeState.State->ourDirectKick ( ) ) || ( worldState ->refereeState.State->ourIndirectKick ( ) ) )
		{
			/*if ( ( ball.Position.X * side < -2500 ) )
			{
				//if ( randomParam < 0.5 )
					corner_simple_chip();
				//else
				//	corner_switch_pass();
			}
			else if ( ( ball.Position.X * side < 0 ) )
			{
				if ( randomParam < 0.5 )
					corner_simple_pass();
				else
					corner_switch_pass();
			}
			else
			{
				throwin_chip_shoot();
				//corner_simple_pass();
			}*/
			corner_simple_chip();
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
			GK ( gk , 2 );
			TwoDef ( def1 , def2 );
			
			OwnRobot[passgir].face ( Vec2 ( -side*2995 , 0 ) );
			ERRTSetObstacles ( passgir );
			ERRTNavigate2Point ( passgir , Vec2 ( -side*1000 , 0 ) );
			
			if ( worldState ->refereeState.State->canKickBall() )
			{
				if ( calculateOpenAngleToGoal(ball.Position, attack).Y >= 70 )
					PenaltyUs(attack,90-sgn(side)*90 ,15);
				else
				{
					if ( randomParam < 0.5 )
						PenaltyUs(attack,AngleWith ( Vec2 ( -side*2995,-220 ) , ball.Position ) ,15);
					else
						PenaltyUs(attack,AngleWith ( Vec2 ( -side*2995,220 ) , ball.Position ) ,15);
				}
				
			}
			else
			{
				PenaltyUs(attack,90-sgn(side)*90 ,0);
			}
		}
		
		else if ( worldState ->refereeState.State->theirFreeKick() )
		{
			if ( 1)//ball.Position.X * side > 2000 )
			{
				//corner_their_marker_ajor();
				corner_their_khafan();
			}
			else
			{
				throwin_their_khafan();
				//corner_their_khafan();
				//corner_their_marker_karkas();
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
		}
		else if( worldState ->refereeState.State->get() == GameState::HALTED )
		{
			HaltAll();
		}
		else
		{
			Stop ( gk , def1 , def2 , passgir , attack );
		}
	}
	else
	{
		Stop ( gk , def1 , def2 , passgir , attack );
		//OwnRobot[4].sendPID ( 0 , 0 , 500 , 1425 );
	}
	
	for ( int i = 0 ; i < 5 ; i ++ )
	{
		if ( OwnRobot[i].State.seenState == CompletelyOut )
		{
			Halt ( i );
		}
	}
	
	internalFinalize(worldState, setting,commands);
}