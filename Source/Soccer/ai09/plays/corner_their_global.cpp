#include "../ai09.h"

void ai09::corner_their_global ( void )
{
	GKHi(gk, 1, 0);
	DefHi(def,NULL, true);
	isDefending = true;
	MarkManager();
	
	for (map<int*,int>::const_iterator i=markMap.begin(); i!=markMap.end(); ++i) {
		int opp = i->second;
		int own = *i->first;
		
		if (oneTouchDetector[own].IsArriving()) {
			WaitForPass(own, false);
		}
		else
		{
			if (opp==-1) {
				OwnRobot[own].face(Vec2(-side*3025, 0));
				ERRTSetObstacles(own, 1, 1, 1, 1, 0, 0);
				if (own==dmf) {
					ERRTNavigate2Point(dmf, Vec2(side*300, -1600), 0, 100, &VELOCITY_PROFILE_MAMOOLI);
				}
				else if (own==mid1) {
					ERRTNavigate2Point(mid1, Vec2(0, -1600), 0, 100, &VELOCITY_PROFILE_MAMOOLI);
				}
				else if (own==mid2) {
					ERRTNavigate2Point(mid2, Vec2(0, 1600), 0, 100, &VELOCITY_PROFILE_MAMOOLI);
				}
			}
			else {
				OwnRobot[own].face(ball.Position);
				Mark2Goal(own,opp,200);
			}
		}
	}
	
	DefenceWall(attack, false);
}