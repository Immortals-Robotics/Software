#include "ai09.h"

void ai09::WaitForOmghi ( int robot_num , bool chip )
{
	Line ball_line = Line::makeLineFromPositionAndAngle ( VecPosition ( ball.Position.X , ball.Position.Y ) , ball.velocity.direction );
	if (chip_head<180) {
		ball_line = Line::makeLineFromPositionAndAngle ( VecPosition ( ball.Position.X , ball.Position.Y ) , chip_head );
		cout << "	calcing with static head: " << chip_head << endl;
	}
	Line to_goal_line = Line::makeLineFromTwoPoints(VecPosition(OwnRobot[robot_num].State.Position.X,OwnRobot[robot_num].State.Position.Y) , VecPosition(-side*field_width,0) );
	
	VecPosition ans = ball_line.getIntersection ( to_goal_line );
	
	float sBAR;
	sBAR = ans.getDistanceTo ( VecPosition ( ball.Position.X , ball.Position.Y ) );
	sBAR /= ball.velocity.magnitude;
	sBAR = ans.getDistanceTo ( VecPosition ( OwnRobot[robot_num].State.Position.X , OwnRobot[robot_num].State.Position.Y ) ) / sBAR;
	sBAR /= 63.0;
	//sBAR /= 10.0;
	//sBAR /= 1500000;
	
	cout << "old sBAR:	" << sBAR << "	"
	;
	if ( sBAR < 5 )		sBAR = 5;
	if ( sBAR > 70 )	sBAR = 70;
	
	TVec2 target = Vec2(ans.getX(), ans.getY());//CalculatePassPos(robot_num, 89);
	
	OwnRobot[robot_num].target.Angle = calculateOneTouchAngle ( robot_num , target );
	OwnRobot[robot_num].face(Vec2(-side*field_width, -sgn(OwnRobot[robot_num].State.Position.Y)*300));
	
	ERRTSetObstacles ( robot_num );
	
	target = CalculatePassPos(robot_num,Vec2(-side*field_width, 0), OwnRobot[robot_num].State.Position, -200);
	
	cout << "sBAR:	" << sBAR << endl;
	ERRTNavigate2Point ( robot_num , target , 0 , sBAR , &VELOCITY_PROFILE_KHARAKI);
	
	OwnRobot[robot_num].Shoot( 100 );
	OwnRobot[robot_num].Dribble( 15 );
}