#include "ai09.h"

#define world2mapX(a) min(729,max(0,((a/10)+364)))
#define world2mapY(a) min(529,max(0,((a/10)+264)))

void ai09::Mark2Ball ( int robot_num , int opp , float dist )
{
	const float opp_predict_t = 0.15;
	dist = min ( 1500 , dist );
	float max_dist = DIS(OppRobot[opp].Position, ball.Position) - 95;
	max_dist = max ( dist , max_dist );
	
	ERRTSetObstacles(robot_num, 0, 1, 0, 0, 0, 0);
	TVec2 target = PointOnConnectingLine ( Vec2(OppRobot[opp].Position.X + OppRobot[opp].velocity.x*opp_predict_t,OppRobot[opp].Position.Y + OppRobot[opp].velocity.y*opp_predict_t ) , ball.Position , 190 );
	if ( IsInObstacle(Vec2(world2mapX(target.X), world2mapY(target.Y))) ) {
		ERRTSetObstacles(robot_num, 0, 1, 1, 0, 0, 0);
	}
	else {
		ERRTSetObstacles(robot_num, 0, 1, 1, 0, 0, 0);
		AddCircle ( OppRobot[opp].Position.X , OppRobot[opp].Position.Y , 15 );
		dist = max ( 16 , dist );
	}
	
	while ( (dist <= 1500) && ( dist <= max_dist ) ) {
		target = PointOnConnectingLine ( Vec2(OppRobot[opp].Position.X + OppRobot[opp].velocity.x*opp_predict_t,OppRobot[opp].Position.Y + OppRobot[opp].velocity.y*opp_predict_t ) , ball.Position , dist );
		
		if ( IsInObstacle(Vec2(world2mapX(target.X), world2mapY(target.Y))) )
		{
			dist += 100;
		}
		else {
			break;
		}
		
	}
	
	ERRTNavigate2Point(robot_num, target, 0, 100, &VELOCITY_PROFILE_KHARAKI);
}