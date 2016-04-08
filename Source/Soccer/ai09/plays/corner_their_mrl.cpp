#include "../ai09.h"

void ai09::corner_their_mrl ( void )
{
	if (side*ball.Position.X > field_width - 1000)
	{
		ERRTSetObstacles(gk, 0, 0, 1, 0, 0, 0);
		OwnRobot[gk].target.Angle = (1 + side)*90.0f;
		ERRTNavigate2Point(gk, Vec2(side*(field_width - 100), 0), 0, 100, &VELOCITY_PROFILE_MAMOOLI);

		ERRTSetObstacles(def, 1, 1, 1, 1, 0, 0);
		OwnRobot[def].face(ball.Position);
		auto defTarget = PointOnConnectingLine(Vec2(side*field_width, 0), ball.Position, 2000);
		ERRTNavigate2Point(def, defTarget, 0, 100, &VELOCITY_PROFILE_MAMOOLI);
	}
	else
	{
		GKHi(gk, 1, 0);
		DefHi(def, NULL, true);
	}

	isDefending = true;
	DefenceWall(attack, false);
	
	map<int,TVec2> static_pos;
	static_pos[dmf] = Vec2(side*3500, -sgn(ball.Position.Y)*1100);
	static_pos[mid1] = Vec2(side*3200, 600);
	static_pos[mid2] = Vec2(side*3200, 0);
	
	int gooshe = findGusheRobot(-1);
	markMap[&dmf] = gooshe;
	if (gooshe != -1) {
		Mark2Goal(dmf, gooshe, 180);
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
	}
	else {
		float mark_dis = min(2000, DIS(Vec2(side*field_width, 0), OppRobot[jelos[0]].Position)-180);
		TVec2 markPoint = PointOnConnectingLine(Vec2(side*field_width, 0), OppRobot[jelos[0]].Position, mark_dis);
		
		ERRTSetObstacles(mid1, 1, 1, 1, 1, 0, 0);
		ERRTNavigate2Point(mid1, markPoint, 0, 100, &VELOCITY_PROFILE_KHARAKI);
		OwnRobot[mid1].face(ball.Position);
		markMap[&mid1] = jelos[0];
	}
	
	
	int remaining_jelos[12];
	int remaining_jelos_num = findJeloOpps(-1000, remaining_jelos, gooshe,jelos[0], 0, 1);
	cout << "	remaining jelos_num: " << remaining_jelos_num << endl;
	
	if (remaining_jelos_num == 0) {
		ERRTSetObstacles(mid2, 1, 1, 1, 1, 0, 0);
		ERRTNavigate2Point(mid2, static_pos[mid2], 0, 100, &VELOCITY_PROFILE_MAMOOLI);
		OwnRobot[mid2].face(ball.Position);
		markMap[&mid2] = -1;
	}
	else {
		float mark_dis = min(2500, DIS(Vec2(side*field_width, 0), OppRobot[remaining_jelos[0]].Position)-180);
		TVec2 markPoint = PointOnConnectingLine(Vec2(side*field_width, 0), OppRobot[remaining_jelos[0]].Position, mark_dis);
		
		ERRTSetObstacles(mid2, 1, 1, 1, 1, 0, 0);
		ERRTNavigate2Point(mid2, markPoint, 0, 100, &VELOCITY_PROFILE_KHARAKI);
		OwnRobot[mid2].face(ball.Position);
		markMap[&mid2] = remaining_jelos[0];
	}
}