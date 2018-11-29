#include "ai09.h"
#include <math.h>

void ai09::OneDef ( int robot_num , TVec2 * defendTarget , bool stop )
{
	if ( !defendTarget )
		defendTarget = &(ball.Position);
	
	ERRTSetObstacles ( robot_num , stop , true , true , false );
	OwnRobot[robot_num].face ( Vec2 ( (*defendTarget).X , (*defendTarget).Y ) );
	ERRTNavigate2Point ( robot_num , PointOnConnectingLine(Vec2(side*3025.0f,-200.0f*sgn((*defendTarget).Y+100.0f)),Vec2((*defendTarget).X,(*defendTarget).Y),1000.0f) );
}