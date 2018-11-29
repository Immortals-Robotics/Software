#include "../ai09.h"

void ai09::corner_their_skuba ( void )
{
	//GKHi(gk, 1, 0);
	ERRTSetObstacles(gk, 0, 0, 1, 0, 0, 0);
	OwnRobot[gk].target.Angle = (1+side)*90.0f;
	ERRTNavigate2Point(gk, Vec2(side*2900.0f, 0), 0, 100, &VELOCITY_PROFILE_MAMOOLI);
	
	DefHi(def,NULL, true);
	isDefending = true;
	DefenceWall(attack, false);
	
	map<int,TVec2> static_pos;
	static_pos[dmf] = Vec2(side*2300, -sgn(ball.Position.Y)*800);
	static_pos[mid1] = Vec2(side*2000, 300);
	static_pos[mid2] = Vec2(side*2000, 300);
	
	int gooshe = findGusheRobot(-1);
	markMap[&dmf] = gooshe;
	if (gooshe != -1) {
		Mark2Goal(dmf, gooshe, 60);
	}
	else {
		ERRTSetObstacles(dmf, 1, 1, 1, 1, 0, 0);
		ERRTNavigate2Point(dmf, static_pos[dmf], 0, 100, &VELOCITY_PROFILE_MAMOOLI);
		OwnRobot[dmf].face(ball.Position);
	}
	
	int jelos[12];
	int jelos_num = findJeloOpps(-1000, jelos, gooshe,-1, 0,0);
	
	cout << "	jelos_num: " << jelos_num << endl;
	
	if (jelos_num == 0) {
		ERRTSetObstacles(mid1, 1, 1, 1, 1, 0, 0);
		ERRTNavigate2Point(mid1, static_pos[mid1], 0, 100, &VELOCITY_PROFILE_MAMOOLI);
		OwnRobot[mid1].face(ball.Position);
		markMap[&mid1] = -1;
		
		ERRTSetObstacles(mid2, 1, 1, 1, 1, 0, 0);
		ERRTNavigate2Point(mid2, static_pos[mid2], 0, 100, &VELOCITY_PROFILE_MAMOOLI);
		OwnRobot[mid2].face(ball.Position);
		markMap[&mid2] = -1;
	}
	else if (jelos_num == 1) {
		float mark_dis = min(2000, DIS(Vec2(side*3025, 0), OppRobot[jelos[0]].Position)-50);
		TVec2 markPoint = PointOnConnectingLine(Vec2(side*3025, 0), OppRobot[jelos[0]].Position, mark_dis);
		
		ERRTSetObstacles(mid1, 1, 1, 1, 0, 0, 0);
		ERRTNavigate2Point(mid1, markPoint, 0, 100, &VELOCITY_PROFILE_KHARAKI);
		OwnRobot[mid1].face(ball.Position);
		markMap[&mid1] = jelos[0];
		
		ERRTSetObstacles(mid2, 1, 1, 1, 0, 0, 0);
		ERRTNavigate2Point(mid2, markPoint, 0, 100, &VELOCITY_PROFILE_KHARAKI);
		OwnRobot[mid2].face(ball.Position);
		markMap[&mid2] = jelos[0];
	}
	else {
		if (OppRobot[jelos[0]].Position.Y>OppRobot[jelos[1]].Position.Y) {
			swap(jelos[0], jelos[1]);
		}
		
		float mark_dis = min(2000, DIS(Vec2(side*3025, 0), OppRobot[jelos[0]].Position)-50);
		TVec2 markPoint = PointOnConnectingLine(Vec2(side*3025, 0), OppRobot[jelos[0]].Position, mark_dis);
		
		ERRTSetObstacles(mid1, 1, 1, 1, 0, 0, 0);
		ERRTNavigate2Point(mid1, markPoint, 0, 100, &VELOCITY_PROFILE_KHARAKI);
		OwnRobot[mid1].face(ball.Position);
		markMap[&mid1] = jelos[0];
		
		
		mark_dis = min(2000, DIS(Vec2(side*3025, 0), OppRobot[jelos[1]].Position)-50);
		markPoint = PointOnConnectingLine(Vec2(side*3025, 0), OppRobot[jelos[1]].Position, mark_dis);
		
		ERRTSetObstacles(mid2, 1, 1, 1, 0, 0, 0);
		ERRTNavigate2Point(mid2, markPoint, 0, 100, &VELOCITY_PROFILE_KHARAKI);
		OwnRobot[mid2].face(ball.Position);
		markMap[&mid2] = jelos[1];
	}



}