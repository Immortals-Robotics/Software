#include "ai09.h"

static int attackerChangeHys = 0;

void ai09::ManageAttRoles ( void )
{
	if ( 
		(DIS(OwnRobot[attack].State.Position, ball.Position)>600)|| //Check if the current attacker has lost the ball, before switching its role
		(OwnRobot[attack].State.seenState == CompletelyOut )
		)
	{
		attackerChangeHys ++;
	}
	
	if (attackerChangeHys > 30) {
		int newAttack = attack;
		
		float currAttBallDis = DIS(OwnRobot[newAttack].State.Position, ball.Position);
		
		if(OwnRobot[attack].State.seenState == CompletelyOut )
		{
			currAttBallDis = 10000;
		}
		
		if ( OwnRobot[lmf].State.seenState != CompletelyOut )
		if (DIS(OwnRobot[lmf].State.Position, ball.Position) < currAttBallDis) {
			if (!oneTouchDetector[lmf].IsArriving(70)) {
				currAttBallDis = DIS(OwnRobot[lmf].State.Position, ball.Position);
				newAttack = lmf;
			}
		}
		
		if ( OwnRobot[rmf].State.seenState != CompletelyOut )
		if (DIS(OwnRobot[rmf].State.Position, ball.Position) < currAttBallDis) {
			if (!oneTouchDetector[rmf].IsArriving(70)) {
				currAttBallDis = DIS(OwnRobot[rmf].State.Position, ball.Position);
				newAttack = rmf;
			}
		}
		
		if ( OwnRobot[cmf].State.seenState != CompletelyOut )
		if (DIS(OwnRobot[cmf].State.Position, ball.Position) < currAttBallDis) {
			if (!oneTouchDetector[cmf].IsArriving(70)) {
				currAttBallDis = DIS(OwnRobot[cmf].State.Position, ball.Position);
				newAttack = cmf;
			}
		}
		
		if ( attack != newAttack )
		{
			if (mid1 == newAttack) {
				mid1 = attack;
			}
			else if (mid2 == newAttack) {
				mid2 = attack;
			}
			
			attackerChangeHys = 0;
			attack = newAttack;
		}
	}
	
	if (OwnRobot[mid1].State.Position.Y<OwnRobot[mid2].State.Position.Y) {
		swap(mid1, mid2);
	}
}