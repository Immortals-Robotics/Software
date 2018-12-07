#include "../ai09.h"

void ai09::penalty_their_simple ( void )
{
    float penalty_x = field_width - 85.0;

	static VelocityProfile VELOCITY_PROFILE_KILLER= VELOCITY_PROFILE_KHARAKI;
	VELOCITY_PROFILE_KILLER.max_spd=Vec2(40.0f);
	VELOCITY_PROFILE_KILLER.max_dec=Vec2(1.8f);
	VELOCITY_PROFILE_KILLER.max_acc=Vec2(1.3f);

	int index = findKickerOpp(-1);
	if ( index == -1 )
	{
		OwnRobot[gk].target.Angle = (1+side)*90.0f;
		Navigate2Point ( gk , Vec2 ( side*penalty_x , 0 ) );
	}
	else
	{
		float gkp_y = Line::makeLineFromTwoPoints ( VecPosition ( OppRobot[index].Position.X , OppRobot[index].Position.Y ) , VecPosition ( ball.Position.X , ball.Position.Y ) ).getIntersection ( Line::makeLineFromTwoPoints ( VecPosition ( side * penalty_x , 100 ) , VecPosition ( side * penalty_x , -100 ) ) ).getY ( );
        float max_reach_y = (goal_width/2.0) - 50.0;
		if(max_reach_y < gkp_y)
			gkp_y = max_reach_y;
		if(-max_reach_y > gkp_y)
			gkp_y = -max_reach_y;
		
		OwnRobot[gk].face(ball.Position);

		Navigate2Point ( gk , Vec2 ( side * penalty_x , gkp_y ),false,100,&VELOCITY_PROFILE_KILLER );
	}
	ERRTSetObstacles ( lw , true , true , true , true );
	ERRTNavigate2Point ( lw , Vec2 ( side*2000 , 500 ), false, 80, &VELOCITY_PROFILE_AROOM );
	ERRTSetObstacles ( rw , true , true , true , true );
	ERRTNavigate2Point ( rw , Vec2 ( side*2000 , -500 ), false, 80, &VELOCITY_PROFILE_AROOM );
	ERRTSetObstacles ( def , true , true , true , true );
	ERRTNavigate2Point ( def , Vec2 ( side*2000 , 800 ), false, 80, &VELOCITY_PROFILE_AROOM );
	ERRTSetObstacles ( dmf , true , true , true , true );
	ERRTNavigate2Point ( dmf , Vec2 ( side*2000 , -800 ), false, 80, &VELOCITY_PROFILE_AROOM );
	ERRTSetObstacles ( mid1 , true , true , true , true );
	ERRTNavigate2Point ( mid1, Vec2 ( side*2000 , -1500 ), false, 80, &VELOCITY_PROFILE_AROOM );
	ERRTSetObstacles ( mid2 , true , true , true , true );
	ERRTNavigate2Point ( mid2, Vec2 ( side*2000 , 1500 ), false, 80, &VELOCITY_PROFILE_AROOM );
	ERRTSetObstacles ( attack , true , true , true , true );
	ERRTNavigate2Point ( attack , Vec2 ( side*2000 , 0 ), false, 80, &VELOCITY_PROFILE_AROOM );
}