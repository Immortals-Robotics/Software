//
// Created by dot_blue on 12/7/18.
//

#include "ai09.h"
#include <math.h>

void ai09::GKHi_Simple(int robot_num, bool stop) {

    float penalty_x = field_width - 85.0;
    float max_reach_y = (goal_width/2.0) + 20.0;

    float thr_ball_vel = 80;
    bool BallisMoving;
    bool stoppedBallinGoalArea;
    bool hurryup = ballIsGoaling();
    static VelocityProfile VELOCITY_PROFILE_KILLER= VELOCITY_PROFILE_KHARAKI;
    VELOCITY_PROFILE_KILLER.max_spd=Vec2(70.0f);
    VELOCITY_PROFILE_KILLER.max_dec=Vec2(2.0f);
    VELOCITY_PROFILE_KILLER.max_acc=Vec2(1.5f);

    debugDraw = true;
    if(hurryup){
        AddDebugCircle(OwnRobot[robot_num].State.Position,200,Red);
    }else{
        AddDebugCircle(OwnRobot[robot_num].State.Position,20,Yellow);
    }
    debugDraw = false;

//    static int hys = 10;
    std::cout<<"Ball Velocity: "<<ball.velocity.magnitude<< std::endl;
    std::cout<<"hurryup: "<<hurryup<< std::endl;
    std::cout<<"_POS_XY: "<<ball.Position.X<<" _ "<<ball.Position.Y<< std::endl;

    if(ball.velocity.magnitude < thr_ball_vel){//Ball is stationary
        BallisMoving = false;
    } else {
        BallisMoving = true;
    }

//    stoppedBallinGoalArea = ( !BallisMoving ) && ( ball.Position.X < side*(field_width - 1200) )

    if(stop){

    }else{
        Line GK_Rail = Line::makeLineFromTwoPoints(VecPosition(side * penalty_x, 100),
                                                   VecPosition(side * penalty_x, -100));
        VecPosition ans;

        if(hurryup){
//            Line ball_line = Line::makeLineFromPositionAndAngle ( VecPosition ( ball.Position.X , ball.Position.Y ) ,
//                                                                  ball.velocity.direction );
//            ans = GK_Rail.getIntersection(ball_line);
            GK_shirje_2018(robot_num,&VELOCITY_PROFILE_KILLER);
        }else {
            float tmp_x,tmp_y;
//            ans = GK_Rail.getPointOnLineClosestTo(VecPosition(ball.Position.X, ball.Position.Y));


            OwnRobot[robot_num].face(ball.Position);
//            float tmp_x = ans.getX();
//            float tmp_y = ans.getY();

            //ASADIs_Idea:
            if(DIS(ball.Position,Vec2(side*field_width,0))<2500 && ball.Position.X > side*(field_width -1200) ){
                ans = GK_Rail.getPointOnLineClosestTo(VecPosition(ball.Position.X, ball.Position.Y));
                tmp_x = ans.getX();
                tmp_y = ans.getY();
            } else {
                tmp_x = side * penalty_x;
                tmp_y = (6.0 * ball.Position.Y) / 40.0f;
            }

            if (tmp_y < -max_reach_y) {
                tmp_y = -max_reach_y;
            }
            if (tmp_y > max_reach_y) {
                tmp_y = max_reach_y;
            }

            TVec2 fans = Vec2(tmp_x, tmp_y);
            std::cout << "__XY__" << tmp_x << "_" << tmp_y << std::endl;
            Navigate2Point(robot_num, fans, 0, 100, stop ? &VELOCITY_PROFILE_AROOM : &VELOCITY_PROFILE_KILLER);
        }


    }


}

void ai09::GK_shirje_2018 ( int robot_num, VelocityProfile* VELOCITY_PROFILE )
{
    Line ball_line = Line::makeLineFromPositionAndAngle ( VecPosition ( ball.Position.X , ball.Position.Y ) , ball.velocity.direction );
    VecPosition ans = ball_line.getPointOnLineClosestTo ( VecPosition ( OwnRobot[robot_num].State.Position.X , OwnRobot[robot_num].State.Position.Y ) );
    OwnRobot[robot_num].face(ball.Position);

    TVec2 fans = Vec2(ans.getX(), ans.getY());
    fans = ((fans-OwnRobot[robot_num].State.Position)*2.0f)+OwnRobot[robot_num].State.Position;
    ERRTSetObstacles ( robot_num ,0,0,0,0);
    ERRTNavigate2Point(robot_num,fans , 1, 100, VELOCITY_PROFILE);
    OwnRobot[robot_num].Chip(150);
}
