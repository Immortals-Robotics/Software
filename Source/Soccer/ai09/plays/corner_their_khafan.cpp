#include "../ai09.h"

void ai09::corner_their_khafan ( void )
{	
	bool defenderMarching = false;
	
	side = -side;
	int oppGK = findCruncherOpp(-1, -1);
	int oppDef1 = findCruncherOpp(oppGK, -1);
	
	if ( ( DIS(OppRobot[oppGK].Position, OppRobot[oppDef1].Position) > 1000 ) && ( DIS(OppRobot[oppGK].Position, OppRobot[oppDef1].Position) < 1500 ) )
	{
		defenderMarching = true;
		marchingDefender = oppDef1;
	}
	side = -side;
	
	if ( ( defenderMarching ) && ( playingAgainstSkuba ) )
	{
		GK(gk, 1);
		TwoDef(def, dmf, &OppRobot[marchingDefender].Position);
		Halt(lmf);
		ERRTNavigate2Point ( rmf , PointOnConnectingLine ( OppRobot[marchingDefender].Position , Vec2 ( side*2995 , 0 ) , 320.0f ) );
		DefenceWall(attack);
	}
	
	else
	{
		int attackingOpps = 0;
		
		for ( int i = 0 ; i < 12 ; i ++ )
		{
			if ( OppRobot[i].seenState == CompletelyOut )
				continue;
			if ( ( fabs ( OppRobot[i].Position.X ) > 3025 ) ||
				( fabs ( OppRobot[i].Position.Y ) > 2025 ) )
				continue;
			if ( ( OppRobot[i].Position.X * side > -500 ) && ( DIS(OppRobot[i].Position, ball.Position) > 600 ) )
			{
				attackingOpps ++;
			}
		}
		cout << "	attacking : " << attackingOpps << endl;
		
		if ( attackingOpps == 0 )
		{
			corner_their_marker_ajor();
		}
		else if ( attackingOpps == 1 )
		{
			corner_their_marker_ajor();
		}
		else if ( attackingOpps == 2 )
		{
			int attackerVasat , attackerGogshe;
			attackerVasat = findCruncherOpp(-1);
			attackerGogshe = findCruncherOpp(attackerVasat);
			
			if ( ( attackerVasat == -1 ) || ( attackerGogshe == -1 ) )
				corner_their_marker_ajor();
			else
			{
				GK(gk,1);
				DefenceWall(attack);
				
				if ( fabs(OppRobot[attackerGogshe].Position.Y) < fabs(OppRobot[attackerVasat].Position.Y) )
				{
					int tmp = attackerVasat;
					attackerVasat = attackerGogshe;
					attackerGogshe = tmp;
				}
				TwoDef(def, dmf, &OppRobot[attackerVasat].Position);
				Halt(lmf);
				
				ERRTSetObstacles ( rmf , 1 );
				if ( fabs(OppRobot[attackerGogshe].Position.X) > 2500 )
					ERRTNavigate2Point ( rmf , PointOnConnectingLine ( OppRobot[attackerGogshe].Position , Vec2 ( side*2995 , 0 ) , 220.0f ) );
				else
					ERRTNavigate2Point ( rmf , PointOnConnectingLine ( OppRobot[attackerGogshe].Position , Vec2 ( side*2995 , 0 ) , 520.0f ) );			}
		}
		else if ( attackingOpps >= 3 )
		{
			int attackerVasat1 , attackerVasat2 , attackerGogshe;
			attackerVasat1 = findCruncherOpp(-1, -1);
			attackerVasat2 = findCruncherOpp(attackerVasat1, -1);
			attackerGogshe = findCruncherOpp(attackerVasat1, attackerVasat2);
			
			if ( ( attackerVasat1 == -1 ) || ( attackerVasat2 == -1 ) || ( attackerGogshe == -1 ) )
				corner_their_marker_ajor();
			else
			{
				GK(gk,1);
				DefenceWall(attack);
				
				if ( fabs(OppRobot[attackerVasat2].Position.Y) < fabs(OppRobot[attackerVasat1].Position.Y) )
				{
					int tmp = attackerVasat1;
					attackerVasat1 = attackerVasat2;
					attackerVasat2 = tmp;
				}
				if ( fabs(OppRobot[attackerGogshe].Position.Y) < fabs(OppRobot[attackerVasat1].Position.Y) )
				{
					int tmp1 = attackerVasat1;
					int tmp2 = attackerVasat2;
					attackerVasat1 = attackerGogshe;
					attackerVasat2 = tmp1;
					attackerGogshe = tmp2;
				}
				else if ( fabs(OppRobot[attackerGogshe].Position.Y) < fabs(OppRobot[attackerVasat2].Position.Y) )
				{
					int tmp = attackerGogshe;
					attackerGogshe = attackerVasat2;
					attackerVasat2 = tmp;
				}
				
				Halt(lmf);
				
				ERRTSetObstacles ( rmf , 1 );
				if ( fabs(OppRobot[attackerGogshe].Position.X) > 2500 )
					ERRTNavigate2Point ( rmf , PointOnConnectingLine ( OppRobot[attackerGogshe].Position , Vec2 ( side*2995 , 0 ) , 220.0f ) );
				else
					ERRTNavigate2Point ( rmf , PointOnConnectingLine ( OppRobot[attackerGogshe].Position , Vec2 ( side*2995 , 0 ) , 520.0f ) );
				
				if ( OppRobot[attackerVasat2].Position.Y < OppRobot[attackerVasat1].Position.Y )
				{
					int tmp = attackerVasat1;
					attackerVasat1 = attackerVasat2;
					attackerVasat2 = tmp;
				}
				
				OneDef ( def , &OppRobot[attackerVasat2].Position );
				OneDef ( dmf , &OppRobot[attackerVasat1].Position );
			}
		}
		/*else if ( attackingOpps == 4 )
		 {
		 corner_their_marker_ajor();
		 }*/
	}
}