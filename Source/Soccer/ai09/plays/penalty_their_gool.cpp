#include "../ai09.h"

void ai09::penalty_their_gool ( void )
{
	bool canKickBall = bool(currentPlayParam);
	canKickBall = true;
	int index = findCruncherOpp(-1, -1, true);
	
	if (( index == -1 )||(!canKickBall))
	{
		OwnRobot[gk].target.Angle = (1+side)*90.0f;
		Navigate2Point ( gk , Vec2 ( side*(field_width-100) , 0 ) );
	}
	else
	{
		cout << "	Penalty zan harif: " << index << endl;
		if (OppRobot[index].velocity.magnitude>10) {
			Navigate2Point(gk, Vec2(side*(field_width-100), 250.0f*sgn(randomParam-0.5f)), 0, 100, &VELOCITY_PROFILE_KHARAKI);
		}
		else {
			Navigate2Point ( gk , Vec2 ( side*(field_width-100) , 0 ) );
		}

	}
	ERRTSetObstacles ( def , true , true , true , true );
	ERRTNavigate2Point ( def , Vec2 ( side*2000 , -800 ) );
	ERRTSetObstacles ( dmf , true , true , true , true );
	ERRTNavigate2Point ( dmf , Vec2 ( side*2000 , -500 ) );
	Halt(lmf);
	ERRTSetObstacles ( rmf , true , true , true , true );
	ERRTNavigate2Point ( rmf , Vec2 ( side*2000 , 800 ) );
	ERRTSetObstacles ( attack , true , true , true , true );
	ERRTNavigate2Point ( attack , Vec2 ( side*2000 , 500 ) );
}