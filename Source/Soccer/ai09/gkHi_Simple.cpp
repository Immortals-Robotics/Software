//
// Created by dot_blue on 12/7/18.
//

#include "ai09.h"
#include <math.h>

void ai09::GKHi_Simple(int robot_num, bool stop) {

    float penalty_x = field_width - 85.0;
    float max_reach_y = (goal_width/2.0) - 30.0;

    if(stop){

    }else{
        Line GK_Rail= Line::makeLineFromTwoPoints ( VecPosition ( side * penalty_x , 100 ) , VecPosition ( side * penalty_x , -100 ) );
        VecPosition ans = GK_Rail.getPointOnLineClosestTo(VecPosition ( ball.Position.X , ball.Position.Y ));
        OwnRobot[robot_num].face(ball.Position);
        float tmp_x = ans.getX();
        float tmp_y = ans.getY();

        if(tmp_y < -max_reach_y){
            tmp_y = -max_reach_y;
        }
        if(tmp_y > max_reach_y){
            tmp_y = max_reach_y;
        }

        TVec2 fans = Vec2(tmp_x, tmp_y);
        cout<<"__XY__"<<tmp_x<<"_"<<tmp_y<<endl;
        Navigate2Point(robot_num, fans, 0, 100, stop ? &VELOCITY_PROFILE_AROOM : &VELOCITY_PROFILE_KHARAKI);

    }

}


