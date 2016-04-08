#include "../ai09.h"

void ai09::corner_their_global ( void )
{
	if (side*ball.Position.X > field_width - 1000)
	{
		ERRTSetObstacles(gk, 0, 0, 1, 0, 0, 0);
		OwnRobot[gk].target.Angle = (1 + side)*90.0f;
		ERRTNavigate2Point(gk, Vec2(side*(field_width - 100), 0), 0, 100, &VELOCITY_PROFILE_MAMOOLI);

		ERRTSetObstacles(def, 1, 1, 1, 1, 0, 0);
		OwnRobot[def].face(ball.Position);
		auto defTarget = PointOnConnectingLine(Vec2(side*field_width, 0), ball.Position, 1400);
		ERRTNavigate2Point(def, defTarget, 0, 100, &VELOCITY_PROFILE_MAMOOLI);
	}
	else
	{
		GKHi(gk, 1, 0);
		DefHi(def, NULL, true);
	}

	isDefending = true;
	DefenceWall(attack, false);

	map<int, TVec2> static_pos;
	static_pos[dmf] = Vec2(side * 3500, -sgn(ball.Position.Y) * 1100);
	static_pos[mid1] = Vec2(side * 3200, 600);
	static_pos[mid2] = Vec2(side * 3200, 0);

	MarkManager(true);
	
	for (map<int*, int>::const_iterator i = markMap.begin(); i != markMap.end(); ++i) {
		int opp = i->second;
		int own = *i->first;

		if (opp == -1) {
			OwnRobot[own].face(Vec2(-side * field_width, 0));
			ERRTSetObstacles(own, 1, 1, 1, 1, 0, 0);
			ERRTNavigate2Point(own, static_pos[own], 0, 100, &VELOCITY_PROFILE_MAMOOLI);
		}
		else {
			Mark(own, opp, 180);
		}
	}
}