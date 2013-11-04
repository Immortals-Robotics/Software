#include "ai09.h"

void ai09::penalty_their_simple ( void )
{
	float mdis = 500;
	int index = -1;
	for ( int i = 0 ; i < 12 ; i ++ )
	{
		if ( OppRobot[i].seenState == CompletelyOut )
			continue;
		if ( DIS ( ball.Position , OppRobot[i].Position ) < mdis )
		{
			mdis = DIS ( ball.Position , OppRobot[i].Position );
			index = i;
		}
	}
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
	ERRTSetObstacles ( def1 , true , true , true , true );
	ERRTNavigate2Point ( def1 , Vec2 ( side*2000 , -800 ) );
	ERRTSetObstacles ( def2 , true , true , true , true );
	ERRTNavigate2Point ( def2 , Vec2 ( side*2000 , -500 ) );
	ERRTSetObstacles ( passgir , true , true , true , true );
	ERRTNavigate2Point ( passgir , Vec2 ( side*2000 , 800 ) );
	ERRTSetObstacles ( attack , true , true , true , true );
	ERRTNavigate2Point ( attack , Vec2 ( side*2000 , 500 ) );
}