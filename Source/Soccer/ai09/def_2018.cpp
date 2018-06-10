//
// Created by dot_blue on 6/10/18.
//

#include "ai09.h"

void ai09::DefMid ( int robot_num , TVec2 * defendTarget , bool stop ) {

    Line ball_line = Line::makeLineFromTwoPoints ( VecPosition ( ball.Position.X , ball.Position.Y ) , VecPosition ( side*field_width , 0.0 ) );
    Line Front_line = Line::makeLineFromPositionAndAngle ( VecPosition ( side*(field_width-penalty_area_r + 300) , 0 ) , 90.0 );
    VecPosition ans = ball_line.getIntersection(Front_line);
    TVec2 fans = Vec2(ans.getX(), ans.getY());

    ERRTSetObstacles ( robot_num ,0,0,0,0,0,0);
    ERRTNavigate2Point(robot_num,fans , 1, 100, &VELOCITY_PROFILE_MAMOOLI);


//    TVec2 target =

}



