#include "ai09.h"

void ai09::DefenceWall ( int robot_num , bool kickOff )
{
	float x = -side*ball.Position.X;
	x = max ( -3200.0f , min ( 3200.0f , x ) );
	float tetta = -0.000003f*x*x + 0.0016f*x + 90.0f;
	if (kickOff) {
		tetta = 14.0f;
	}
	
	TVec2 target;
	
	//std::cout << "	wall limit: " << tetta << std::endl;
	int index = findKickerOpp ( -1 );
	if ( index == -1 )
	{
		target = CircleAroundPoint(Vec2(ball.Position.X,ball.Position.Y),NormalizeAngle(AngleWith(ball.Position , Vec2(side*field_width,0))),730);
	}
	else
	{
		target = PointOnConnectingLine ( OppRobot[index].Position , ball.Position , 590+DIS ( ball.Position , OppRobot[index].Position ) );
	}
	
	//std::cout << index << std::endl;
	
	float ballAngle = AngleWith(ball.Position, target);
	float firstLeg = AngleWith(ball.Position, Vec2(side*field_width, sgn(ball.Position.Y)*(350.0f)));
	float secLeg = firstLeg - tetta * sgn(ball.Position.Y)*side;
	
	//std::cout << "	ball: " << ballAngle << "	f: " << firstLeg << "	s: " << secLeg << std::endl;
	
	bool isOut = fabs((fabs(NormalizeAngle(ballAngle-firstLeg))+fabs(NormalizeAngle(ballAngle-secLeg))) - tetta ) > 1.0f;
	
	if ( isOut )
	{
		target = CircleAroundPoint(Vec2(ball.Position.X,ball.Position.Y),NormalizeAngle(AngleWith(ball.Position , Vec2(side*field_width,0))),730);
	}
	
	OwnRobot[robot_num].face ( ball.Position );
	ERRTSetObstacles ( robot_num , true , true , true , false );
	ERRTNavigate2Point ( robot_num , target );
}