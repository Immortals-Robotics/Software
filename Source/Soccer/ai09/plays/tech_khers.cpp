#include "../ai09.h"

void ai09::tech_khers_pass ( void )
{	
	map<int,TVec2> rcv_pos;
	rcv_pos[dmf] = Vec2(-2500, 1000);
	rcv_pos[rmf] = Vec2(-500, 1000);
	rcv_pos[lmf] = Vec2(-2500, -1000);
	rcv_pos[cmf] = Vec2(-500, -1000);
	
	float min_dis = 6000;
	int attack_index = -1;
	
	int curr_rcv = -1;
	static int last_rcv = -1;
	
	if (last_rcv!=-1) {
		if (OwnRobot[last_rcv].State.seenState != CompletelyOut) {
			if (oneTouchDetector[last_rcv].IsArriving(40, 80)) {
				curr_rcv = last_rcv;
			}
		}
	}
	
	if (curr_rcv == -1) {
		for (map<int,TVec2>::const_iterator it = rcv_pos.begin(); it != rcv_pos.end(); it++) {
			if (it->first == attack_index) {
				continue;
			}
			if (OwnRobot[it->first].State.seenState == CompletelyOut) {
				continue;
			}
			
			if (oneTouchDetector[it->first].IsArriving(40, 80)) {
				curr_rcv = it->first;
			}
		}
	}
	
	static int next_rcv = -1;
	
	if ( (next_rcv == -1) || ( last_rcv != curr_rcv ) ){
		last_rcv = curr_rcv;
		for (map<int,TVec2>::const_iterator it = rcv_pos.begin(); it != rcv_pos.end(); it++) {
			if (it->first == attack_index) {
				continue;
			}
			if (OwnRobot[it->first].State.seenState == CompletelyOut) {
				continue;
			}
			
			if (DIS(OppRobot[findNearestAsshole(it->second, -1, 1)].Position,it->second) < 500) {
				continue;
			}
			
			bool blocked = false;
			Line to_rcv_line = Line::makeLineFromTwoPoints(VecPosition(ball.Position.X,ball.Position.Y),VecPosition(it->second.X,it->second.Y)); 
			for (int i = 0; i < MAX_ROBOTS; i ++) {
				if ( OppRobot[i].seenState == CompletelyOut )
					continue;
				if ( ( fabs ( OppRobot[i].Position.X ) > 3025 ) ||
					( fabs ( OppRobot[i].Position.Y ) > 2025 ) )
					continue;
				
				if (to_rcv_line.getDistanceWithPoint(VecPosition(OppRobot[i].Position.X,OppRobot[i].Position.Y))<150) {
					blocked = true;
					break;
				}
			}
			
			if (blocked) {
				continue;
			}
			
			next_rcv = it->first;
			break;
		}	
		
	}
	
	if (ball.velocity.magnitude < 50) {
		for (map<int,TVec2>::const_iterator it = rcv_pos.begin(); it != rcv_pos.end(); it++) {
			if (OwnRobot[it->first].State.seenState == CompletelyOut) {
				continue;
			}
			if (oneTouchDetector[it->first].IsArriving(20, 60)) {
				continue;
			}
			if (DIS(it->second, ball.Position) < min_dis) {
				min_dis = DIS(it->second, ball.Position);
				attack_index = it -> first;
			}
		}
	}
	
	if ( attack_index != -1 )
	{
		float pass_angle = AngleWith(OwnRobot[next_rcv].State.Position,OwnRobot[attack_index].State.Position);
		tech_circle(attack_index, pass_angle, 8, 0, 0, 1, 0, 0);
	}
	
	for (map<int,TVec2>::const_iterator it = rcv_pos.begin(); it != rcv_pos.end(); it++) {
		if (it->first == attack_index) {
			continue;
		}
		
		if (oneTouchDetector[it->first].IsArriving(40, 80)) {
			TVec2 target = CalculatePassPos(it->first,OwnRobot[next_rcv].State.Position,OwnRobot[it->first].State.Position, 85);
			//OwnRobot[it->first].face(ball.Position);
			OwnRobot[it->first].face(OwnRobot[next_rcv].State.Position);
			Navigate2Point(it->first, target);
			//OwnRobot[it->first].Dribble(15);
		}
		else {
			Navigate2Point(it->first, it->second);
			OwnRobot[it->first].face(ball.Position);
		}
	}
	
}