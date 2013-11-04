#include "ai09.h"

void ai09::DefenceWall ( int robot_num , float tolerace )
{
	int index = findKickerOpp ( -1 );
	if ( index == -1 )
	{
		ERRTSetObstacles ( attack , true , true , true , false );
		OwnRobot[attack].face(Vec2(ball.Position.X,ball.Position.Y));
		ERRTNavigate2Point ( attack , CircleAroundPoint(Vec2(ball.Position.X,ball.Position.Y),NormalizeAngle(AngleWith(ball.Position , Vec2(side*2995,0))),590) ,0 , 100);
	}
	else
	{
		OwnRobot[attack].face ( ball.Position );
		ERRTSetObstacles ( attack , 1 );
		ERRTNavigate2Point ( attack , PointOnConnectingLine ( OppRobot[index].Position , ball.Position , 590+DIS ( ball.Position , OppRobot[index].Position ) ) );
	}
	
	cout << "	" << 200.0 * cosDeg(OppRobot[index].Angle) << "	";
}