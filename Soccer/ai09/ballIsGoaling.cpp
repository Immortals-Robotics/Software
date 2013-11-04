#include <math.h>
#include "ai09.h"

bool ai09::ballIsGoaling ( void )
{
	bool towardGoal = false;
	
	if ( ( side == -1 ) && ( fabs(ball.velocity.direction) > 90 ) )
		towardGoal = true;
	if ( ( side == 1 ) && ( fabs(ball.velocity.direction) < 90 ) )
		towardGoal = true;
	
	if (!towardGoal)
		return false;
	
	if ( ball.velocity.magnitude < 200 )
		return false;
	
	Line ball_line = Line::makeLineFromPositionAndAngle ( VecPosition ( ball.Position.X , ball.Position.Y ) , ball.velocity.direction );
	Line targetLine = Line::makeLineFromTwoPoints ( VecPosition ( side * 3025 , -100 ) , VecPosition ( side * 3025 , 100 ) );
	VecPosition ballInter = ball_line.getIntersection(targetLine);
	
	if (fabs(ballInter.getY())<440)
		return true;
	
	return false;
}