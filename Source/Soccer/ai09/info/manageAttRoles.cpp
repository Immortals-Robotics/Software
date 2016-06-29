#include "ai09.h"

static int attackerChangeHys = 0;

void ai09::ManageAttRoles ( void )
{
	if (DIS(OwnRobot[attack].State.Position, ball.Position) > 600) //Check if the current attacker has lost the ball, before switching its role
		attackerChangeHys++;
	if (OwnRobot[attack].State.seenState == CompletelyOut)
		attackerChangeHys = 100;
	
	if (attackerChangeHys > 30) {
		int newAttack = attack;

		auto mid1_score = calculateSwicthToAttackerScore(mid1);
		auto mid2_score = calculateSwicthToAttackerScore(mid2);
		if (mid1_score > 0.1f && mid2_score > 0.1f)
		{
			if (mid1_score > mid2_score)
				newAttack = mid1;
			else
				newAttack = mid2;
		}
		else if (mid1_score > 0.1f)
		{
			newAttack = mid1;
		}
		else if (mid2_score > 0.1f)
		{
			newAttack = mid2;
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