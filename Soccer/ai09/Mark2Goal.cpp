#include "ai09.h"

void ai09::Mark2Goal ( int robot_num , int opp , float dist )
{
	const float opp_predict_t = 0.15;
	dist = min ( 1500 , dist );
	float max_dist = DIS(OppRobot[opp].Position, Vec2(side*field_width, 0)) - dist;
	max_dist = max ( dist , max_dist );
	
	ERRTSetObstacles(robot_num, 0, 1, 0, 0, 0, 0);
    
    TVec2 predictedOpp = Vec2(OppRobot[opp].Position.X + OppRobot[opp].velocity.x*opp_predict_t,OppRobot[opp].Position.Y + OppRobot[opp].velocity.y*opp_predict_t );
    
	TVec2 target = PointOnConnectingLine ( predictedOpp , Vec2 ( side*field_width , 0 ) , dist );
	//if ( IsInObstacle(Vec2(target.X, target.Y)) ) {
	//	ERRTSetObstacles(robot_num, 1, 1, 1, 0, 0, 0);
    //    AddOppObs(opp);
	//}
	//else {
		ERRTSetObstacles(robot_num, 1, 1, 1, 1, 0, 0);
	//	dist = max ( 16 , dist );
	//}
	
	/*while ( (dist <= 1500) && ( dist <= max_dist ) ) {
		target = PointOnConnectingLine ( Vec2(OppRobot[opp].Position.X + OppRobot[opp].velocity.x*opp_predict_t,OppRobot[opp].Position.Y + OppRobot[opp].velocity.y*opp_predict_t ) , Vec2 ( side*field_width , 0 ) , dist );
		
		if ( IsInObstacle(Vec2(target.X, target.Y)) )
		{
			dist += 100;
		}
		else {
			break;
		}

	}*/
	
	ERRTNavigate2Point(robot_num, target, 0, 100, &VELOCITY_PROFILE_KHARAKI);
}