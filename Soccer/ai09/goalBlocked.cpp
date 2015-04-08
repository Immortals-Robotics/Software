#include "ai09.h"

bool ai09::goal_blocked ( TVec2 init_pos, float max_shoot_blocker_dis, float shoot_blocker_r )
{
	bool oppGoalOpen = true;
	
	Line ballGoalLine = Line::makeLineFromTwoPoints(VecPosition(-3025*side,0),VecPosition(init_pos.X,init_pos.Y));
	for (int i = 0; i < MAX_ROBOTS; i ++) {
		if ( OppRobot[i].seenState == CompletelyOut )
			continue;
		if ( ( fabs ( OppRobot[i].Position.X ) > field_width ) ||
			( fabs ( OppRobot[i].Position.Y ) > field_height ) )
			continue;
		if (DIS(OppRobot[i].Position, init_pos)>max_shoot_blocker_dis)
			continue;
		
		if (ballGoalLine.getDistanceWithPoint(VecPosition(OppRobot[i].Position.X,OppRobot[i].Position.Y))<shoot_blocker_r) {
			oppGoalOpen = false;
			break;
		}
	}
	/*if (oppGoalOpen) {
		for (int i = 0; i < 6; i ++) {
			if ( OwnRobot[i].State.seenState == CompletelyOut )
				continue;
			if ( ( fabs ( OwnRobot[i].State.Position.X ) > 3025 ) ||
				( fabs ( OwnRobot[i].State.Position.Y ) > 2025 ) )
				continue;
			if (DIS(OwnRobot[i].State.Position, init_pos)>max_shoot_blocker_dis)
				continue;
			
			if (ballGoalLine.getDistanceWithPoint(VecPosition(OwnRobot[i].State.Position.X,OwnRobot[i].State.Position.Y))<shoot_blocker_r) {
				oppGoalOpen = false;
				break;
			}
		}
	}*/
	
	return !oppGoalOpen;
}