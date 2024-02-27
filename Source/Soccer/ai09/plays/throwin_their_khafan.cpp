#include "../ai09.h"

void ai09::throwin_their_khafan ( void )
{
	GK(gk, 1);
	OneDef(def);
	DefenceWall(attack);
	
	Halt(lmf);
	
	int attackerDoor , attackerNazdik;
	attackerNazdik = findCruncherOpp(-1);
	attackerDoor = findCruncherOpp(attackerNazdik);
	
	if ( ( attackerDoor == -1 ) || ( attackerNazdik == -1 ) )
		corner_their_def_ajor();
	else
	{
		GK(gk, 1);
		OneDef(def);
		DefenceWall(attack);
		
		OneDef(dmf, &OppRobot[attackerNazdik].Position);
		
		ERRTSetObstacles ( rmf , 1 );
		ERRTNavigate2Point ( rmf , PointOnConnectingLine ( OppRobot[attackerDoor].Position , Vec2 ( side*2995 , 0 ) , 520.0f ) );
	}
}