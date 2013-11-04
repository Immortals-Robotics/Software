#include "ai09.h"

void ai09::penalty_their_simple ( void )
{
	int index = findKickerOpp(-1);
	if ( index == -1 )
	{
		OwnRobot[gk].target.Angle = (1+side)*90.0f;
		Navigate2Point ( gk , Vec2 ( side*2900.0f , 0 ) );
	}
	else
	{
		float gkp_y = Line::makeLineFromTwoPoints ( VecPosition ( OppRobot[index].Position.X , OppRobot[index].Position.Y ) , VecPosition ( ball.Position.X , ball.Position.Y ) ).getIntersection ( Line::makeLineFromTwoPoints ( VecPosition ( side * 2940 , 100 ) , VecPosition ( side * 2940 , -100 ) ) ).getY ( );
		if(gkp_y > 250)
			gkp_y = 250;
		if(gkp_y < -250)
			gkp_y = -250;
		
		OwnRobot[gk].face(ball.Position);
		Navigate2Point ( gk , Vec2 ( side * 2940 , gkp_y ) );
	}
	ERRTSetObstacles ( def , true , true , true , true );
	ERRTNavigate2Point ( def , Vec2 ( side*2000 , 500 ) );
	ERRTSetObstacles ( dmf , true , true , true , true );
	ERRTNavigate2Point ( dmf , Vec2 ( side*2000 , -500 ) );
	ERRTSetObstacles ( mid1 , true , true , true , true );
	ERRTNavigate2Point ( mid1, Vec2 ( side*2000 , -800 ) );
	ERRTSetObstacles ( mid2 , true , true , true , true );
	ERRTNavigate2Point ( mid2, Vec2 ( side*2000 , 800 ) );
	ERRTSetObstacles ( attack , true , true , true , true );
	ERRTNavigate2Point ( attack , Vec2 ( side*2000 , 0 ) );
}