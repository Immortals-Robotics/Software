#include "ai09.h"

void ai09::throwin_chip_shoot ( void )
{
	if ( ( OwnRobot[passgir].vision_id != 5 ) && ( OwnRobot[def2].vision_id == 5 ) )
	{
		int hehet = OwnRobot[passgir].vision_id;
		OwnRobot[passgir].set_vision_id(OwnRobot[def2].vision_id);
		OwnRobot[def2].set_vision_id(hehet);
	}
	
	GK ( gk , 2 );
	TwoDef ( def1,def2 );
	
	swap ( passgir , attack );
	
	timer.end();
	if ( ( ( DIS ( ball.Position , OwnRobot[attack].State.Position ) < 250 ) && ( OwnRobot[attack].State.velocity.magnitude < 100 ) && (timer.time()>2 )) || ( reached ) )
	{
		tech_circle(attack,AngleWith ( Vec2 ( -side*2995 , 0 ) , ball.Position ) ,0,1008,0,1);
		reached = true;
	}
	else
		tech_circle(attack,AngleWith ( Vec2 ( -side*2995 , 0 ) , ball.Position ) ,0,0,0,1);
	
	OwnRobot[passgir].face ( Vec2 ( -side*2995 , 0 ) );
	ERRTSetObstacles ( passgir , 0,1,1 );
	if ( randomParam < 0.3 )
		ERRTNavigate2Point ( passgir , PointOnConnectingLine ( ball.Position , Vec2 ( -side*2995 , 0 ) , 300+700*reached ) );
	else if ( randomParam < 0.6 )
		ERRTNavigate2Point ( passgir , PointOnConnectingLine ( ball.Position , Vec2 ( -side*2995 , -sgn(ball.Position.X)*2000 ) , 250+700*reached ) );
	else
		ERRTNavigate2Point ( passgir , PointOnConnectingLine ( ball.Position , Vec2 ( -side*2995 , sgn(ball.Position.X)*2000 ) , 250+700*reached ) );
	
	swap ( passgir , attack );
}