

#include "ai09.h"



void ai09::our_place_ball(void) {


    static float move_angle = 0,temp_opp_ang = 180;
    if( DIS(ball.Position,*targetBallPlacement)>100) {
        move_angle = AngleWith(*targetBallPlacement, ball.Position);
        temp_opp_ang = NormalizeAngle(move_angle + 180);
    }

    static float t_ang,t_opp_ang;
    static VelocityProfile TEMP_VEL_PRO = BALL_PLACE_KHEYLI_SOOSKI;

    OwnRobot[cmf].target.velocity.x = 0.0;//TODO added these but never tested
    OwnRobot[cmf].target.velocity.y = 0.0;//TODO added these but never tested
    OwnRobot[rmf].target.velocity.x = 0.0;//TODO added these but never tested
    OwnRobot[rmf].target.velocity.y = 0.0;//TODO added these but never tested
    clear_map ( );
    if(FUNC_state == 0) {
        OwnRobot[cmf].target.Angle = temp_opp_ang;
        OwnRobot[rmf].target.Angle = move_angle;
        ERRTSetObstacles(cmf,0,0,1,1,0,0);
        AddCircle(ball.Position.X, ball.Position.Y, 150.0f);
        ERRTNavigate2Point(cmf, CircleAroundPoint(ball.Position, move_angle, 250), 0, 40, &VELOCITY_PROFILE_AROOM);
        ERRTSetObstacles(rmf,0,0,1,1,0,0);
        AddCircle(ball.Position.X, ball.Position.Y, 150.0f);

        ERRTNavigate2Point(rmf, CircleAroundPoint(ball.Position, temp_opp_ang, 250), 0, 40, &VELOCITY_PROFILE_AROOM);

        if(DIS(CircleAroundPoint ( ball.Position , move_angle , 200 ),OwnRobot[cmf].State.Position) < 100 &&
                DIS(CircleAroundPoint ( ball.Position , temp_opp_ang , 200 ),OwnRobot[rmf].State.Position) < 100){
            FUNC_CNT++;
        }
        if(FUNC_CNT >= 10 ){
            FUNC_state = 1;
            FUNC_CNT = 0;
        }

    }
    else if(FUNC_state == 1){

        Navigate2Point(cmf, CircleAroundPoint(ball.Position, move_angle, 85), 0, 20, &BALL_PLACE_KHEYLI_SOOSKI);

        Navigate2Point(rmf, CircleAroundPoint(ball.Position, temp_opp_ang, 85), 0, 20, &BALL_PLACE_KHEYLI_SOOSKI);


        if(DIS(CircleAroundPoint ( ball.Position , move_angle , 85 ),OwnRobot[cmf].State.Position) < 40 &&
           DIS(CircleAroundPoint ( ball.Position , temp_opp_ang , 85 ),OwnRobot[rmf].State.Position) < 40){
            FUNC_CNT++;
        }
        if(FUNC_CNT >= 10 ){
            FUNC_state = 2;
            FUNC_CNT = 0;

            t_ang = move_angle;
            t_opp_ang = temp_opp_ang;
        }

    }
    else if(FUNC_state == 2){

        OwnRobot[cmf].target.Angle = temp_opp_ang;
        OwnRobot[rmf].target.Angle = move_angle;

        if(DIS(ball.Position,*targetBallPlacement)<500){
            TEMP_VEL_PRO.max_spd = Vec2 ( 300.0f );
        }else if(DIS(ball.Position,*targetBallPlacement)<1000){
            TEMP_VEL_PRO.max_spd = Vec2 ( 600.0f );
        }else if(DIS(ball.Position,*targetBallPlacement)<1500){
            TEMP_VEL_PRO.max_spd = Vec2 ( 900.0f );
        }else{
            TEMP_VEL_PRO.max_spd = Vec2 ( 1300.0f );
        }

        Navigate2Point(cmf, CircleAroundPoint(*targetBallPlacement, move_angle, 170), 0, 20, &TEMP_VEL_PRO);
        Navigate2Point(rmf, CircleAroundPoint(*targetBallPlacement, temp_opp_ang, 170), 0, 20, &TEMP_VEL_PRO);




        bool ball_has_slipt = DIS((OwnRobot[cmf].State.Position + OwnRobot[rmf].State.Position)/2,ball.Position) > 300;

        if(ball.seenState == Seen && ball_has_slipt){
            FUNC_state = 0;
            FUNC_CNT = 0;
        }

        if(DIS(CircleAroundPoint ( *targetBallPlacement , move_angle , 170 ),OwnRobot[cmf].State.Position) < 30 &&
           DIS(CircleAroundPoint ( *targetBallPlacement , temp_opp_ang , 170 ),OwnRobot[rmf].State.Position) < 30){
            FUNC_CNT++;
        }
        if(FUNC_CNT >= 10 ){
            FUNC_state = 3;
            FUNC_CNT = 0;
            timer.start();
        }

    }
    else if(FUNC_state==3){
        if(timer.time()<0.5){
            Navigate2Point(cmf, CircleAroundPoint(*targetBallPlacement, move_angle, 100), 0, 20, &BALL_PLACE_KHEYLI_SOOSKI);
            Navigate2Point(rmf, CircleAroundPoint(*targetBallPlacement, temp_opp_ang, 100), 0, 20, &BALL_PLACE_KHEYLI_SOOSKI);
        }
        else if(timer.time()>1){
            FUNC_state = 4;
            FUNC_CNT = 0;
        }
    }
    else{
//        OwnRobot[attack].face(ball.Position);
//        Navigate2Point(attack,OwnRobot[attack].State.Position,false,0,&BALL_PLACE_KHEYLI_SOOSKI);

        OwnRobot[cmf].face(ball.Position);
        OwnRobot[rmf].face(ball.Position);
        t_ang = NormalizeAngle(OwnRobot[cmf].State.Angle + 180);
        Navigate2Point(cmf, CircleAroundPoint(ball.Position, t_ang, 170), 0, 20, &BALL_PLACE_KHEYLI_SOOSKI);
        t_ang = NormalizeAngle(OwnRobot[rmf].State.Angle + 180);
        Navigate2Point(rmf, CircleAroundPoint(ball.Position, t_ang, 170), 0, 20, &BALL_PLACE_KHEYLI_SOOSKI);

        bool success = DIS(*targetBallPlacement ,ball.Position) < 100.0;

        cout<<"______IN___STATE_DONE_____"
            <<endl
            <<targetBallPlacement->X - ball.Position.X
            <<endl
            <<targetBallPlacement->Y - ball.Position.Y
            <<endl;
        if(success){
            cout<<"MADE it!!!"<<endl;
        }else{
            cout<<"lost it!!!"<<endl;
        }
    }
    cout<<"______IN___STATE_"<< FUNC_state <<"_____"<<endl;


}
void ai09::our_place_ball_shoot(void) {


    static float move_angle,temp_opp_ang;
    if( DIS(ball.Position,*targetBallPlacement)>100) {
        move_angle = AngleWith(*targetBallPlacement, ball.Position);
        temp_opp_ang = NormalizeAngle(move_angle + 180);
    }

    static float t_ang,t_opp_ang;
    static TVec2 t_pos;
    static VelocityProfile TEMP_VEL_PRO = BALL_PLACE_KHEYLI_SOOSKI;

    OwnRobot[cmf].target.velocity.x = 0.0;//TODO added these but never tested
    OwnRobot[cmf].target.velocity.y = 0.0;//TODO added these but never tested
    OwnRobot[rmf].target.velocity.x = 0.0;//TODO added these but never tested
    OwnRobot[rmf].target.velocity.y = 0.0;//TODO added these but never tested
//    clear_map ( );
    if(FUNC_state == 0) {
        if(DIS(ball.Position,*targetBallPlacement) < 1000) {
            FUNC_state = 3;
            FUNC_CNT = 0;
        }

//        OwnRobot[cmf].target.Angle = temp_opp_ang;
        OwnRobot[rmf].target.Angle = move_angle;
//        ERRTSetObstacles(cmf,0,0,1,1,0,0);
//        AddCircle(ball.Position.X, ball.Position.Y, 150.0f);
//        ERRTNavigate2Point(cmf, CircleAroundPoint(ball.Position, move_angle, 250), 0, 40, &VELOCITY_PROFILE_AROOM);

        ERRTSetObstacles(rmf,0,0,1,1,0,0);
        AddCircle(ball.Position.X, ball.Position.Y, 150.0f);
        ERRTNavigate2Point(rmf, CircleAroundPoint(*targetBallPlacement, temp_opp_ang, 250), 0, 40, &VELOCITY_PROFILE_AROOM);

        clear_map ( );
        circle_ball_free(cmf,move_angle,0,0,0.0);
        cout<<":::"<<OwnRobot[cmf].State.velocity.magnitude<<endl<<OwnRobot[rmf].State.velocity.magnitude<<endl;
        if(OwnRobot[cmf].State.velocity.magnitude < 20 &&
           OwnRobot[rmf].State.velocity.magnitude < 20){
            FUNC_CNT++;
        }
        if(FUNC_CNT >= 10 ){
            FUNC_state = 1;
            FUNC_CNT = 0;
        }

    }
    else if(FUNC_state == 1) {

        circle_ball_free(cmf,move_angle,60,0,0.0);
        WaitForPass(rmf,false,&OwnRobot[cmf].State.Position);
//        OwnRobot[rmf].target.Angle = 90;
        if(DIS(OwnRobot[cmf].State.Position,ball.Position) > 400){
            FUNC_CNT++;
        }
        if(FUNC_CNT >= 20 ){
            FUNC_state = 2;
            FUNC_CNT = 0;
            timer.start();
        }

    }
    else if(FUNC_state == 2){

        WaitForPass(rmf,false,&OwnRobot[cmf].State.Position);
//        OwnRobot[rmf].target.Angle = 90;

        if(timer.time() > 2){
            FUNC_CNT++;
        }
        if(FUNC_CNT >= 10 ){
            FUNC_state = 3;
            FUNC_CNT = 0;
            timer.start();
        }
    }
    else if(FUNC_state == 3){

        if(fabs(OwnRobot[cmf].target.Angle - move_angle) > 90)
            swap(move_angle,temp_opp_ang);
        OwnRobot[cmf].target.Angle = move_angle;
        OwnRobot[rmf].target.Angle = temp_opp_ang;
        ERRTSetObstacles(cmf,0,0,1,1,0,0);
        AddCircle(ball.Position.X, ball.Position.Y, 150.0f);
        ERRTNavigate2Point(cmf, CircleAroundPoint(ball.Position, temp_opp_ang, 250), 0, 40, &VELOCITY_PROFILE_AROOM);
        ERRTSetObstacles(rmf,0,0,1,1,0,0);
        AddCircle(ball.Position.X, ball.Position.Y, 150.0f);

        ERRTNavigate2Point(rmf, CircleAroundPoint(ball.Position, move_angle, 250), 0, 40, &VELOCITY_PROFILE_AROOM);

        if(DIS(CircleAroundPoint ( ball.Position , temp_opp_ang , 250 ),OwnRobot[cmf].State.Position) < 100 &&
           DIS(CircleAroundPoint ( ball.Position , move_angle , 250 ),OwnRobot[rmf].State.Position) < 100 &&
               ball.velocity.magnitude < 10 ){

            FUNC_CNT++;
        }
        if(FUNC_CNT >= 10 ){
            FUNC_state = 4;
            FUNC_CNT = 0;
            t_pos = ball.Position;
        }
    }else if(FUNC_state == 4){
        if(fabs(OwnRobot[cmf].target.Angle - move_angle) > 90)
            swap(move_angle,temp_opp_ang);

        Navigate2Point(cmf, CircleAroundPoint(t_pos, temp_opp_ang, 75), 0, 20, &BALL_PLACE_KHEYLI_SOOSKI);

        Navigate2Point(rmf, CircleAroundPoint(t_pos, move_angle, 75), 0, 20, &BALL_PLACE_KHEYLI_SOOSKI);

        bool ball_has_slipt = DIS((OwnRobot[cmf].State.Position + OwnRobot[rmf].State.Position)/2,ball.Position) > 300;

        if(ball.seenState == Seen && ball_has_slipt){
            FUNC_state = 3;
            FUNC_CNT = 0;
        }

        if(DIS(CircleAroundPoint ( t_pos , temp_opp_ang , 75 ),OwnRobot[cmf].State.Position) < 40 &&
           DIS(CircleAroundPoint ( t_pos , move_angle , 75 ),OwnRobot[rmf].State.Position) < 40){
            FUNC_CNT++;
        }
        if(FUNC_CNT >= 10 ){
            FUNC_state = 5;
            FUNC_CNT = 0;
            t_ang = move_angle;
            t_opp_ang = temp_opp_ang;
        }
    }
    else if(FUNC_state == 5){
        Navigate2Point(cmf, CircleAroundPoint(*targetBallPlacement, t_opp_ang, 75), 0, 20, &BALL_PLACE_KHEYLI_SOOSKI);

        Navigate2Point(rmf, CircleAroundPoint(*targetBallPlacement, t_ang, 75), 0, 20, &BALL_PLACE_KHEYLI_SOOSKI);

        bool ball_has_slipt = DIS((OwnRobot[cmf].State.Position + OwnRobot[rmf].State.Position)/2,ball.Position) > 300;

        if(ball.seenState == Seen && ball_has_slipt){
            FUNC_state = 3;
            FUNC_CNT = 0;
        }

        if(DIS(CircleAroundPoint ( *targetBallPlacement , t_opp_ang , 75 ),OwnRobot[cmf].State.Position) < 40 &&
           DIS(CircleAroundPoint ( *targetBallPlacement , t_ang , 75 ),OwnRobot[rmf].State.Position) < 40){
            FUNC_CNT++;
        }
        if(FUNC_CNT >= 30 ){
            FUNC_state = 6;
            FUNC_CNT = 0;
        }
    }
    else if(FUNC_state == 6){

        //OwnRobot[cmf].face(ball.Position);
        Navigate2Point(cmf, CircleAroundPoint(*targetBallPlacement, t_opp_ang, 250), 0, 20, &TEMP_VEL_PRO);
        //OwnRobot[rmf].face(ball.Position);
        Navigate2Point(rmf, CircleAroundPoint(*targetBallPlacement, t_ang, 250), 0, 20, &TEMP_VEL_PRO);



        if(DIS(CircleAroundPoint ( *targetBallPlacement , t_opp_ang , 250 ),OwnRobot[cmf].State.Position) < 40 &&
           DIS(CircleAroundPoint ( *targetBallPlacement , t_ang , 250 ),OwnRobot[rmf].State.Position) < 40){
            FUNC_CNT++;
        }
        if(FUNC_CNT >= 50 ){
            if(DIS(ball.Position,*targetBallPlacement) > 95){
                FUNC_state = 3;
                FUNC_CNT = 0;
            }else {
                FUNC_state = 7;
                FUNC_CNT = 0;
            }
        }
    }
    else{
//        OwnRobot[attack].face(ball.Position);
//        Navigate2Point(attack,OwnRobot[attack].State.Position,false,0,&BALL_PLACE_KHEYLI_SOOSKI);

//        OwnRobot[cmf].target.Angle = 0;
//        OwnRobot[rmf].target.Angle = 0;
//        t_ang = NormalizeAngle(OwnRobot[cmf].State.Angle + 180);
//        Navigate2Point(cmf, CircleAroundPoint(ball.Position, t_ang, 170), 0, 20, &BALL_PLACE_KHEYLI_SOOSKI);
//        t_ang = NormalizeAngle(OwnRobot[rmf].State.Angle + 180);
//        Navigate2Point(rmf, CircleAroundPoint(ball.Position, t_ang, 170), 0, 20, &BALL_PLACE_KHEYLI_SOOSKI);

        bool success = DIS(*targetBallPlacement ,ball.Position) < 100.0;

        cout<<"______IN___STATE_DONE_____"
            <<endl
            <<targetBallPlacement->X - ball.Position.X
            <<endl
            <<targetBallPlacement->Y - ball.Position.Y
            <<endl;
        if(success){
            cout<<"MADE it!!!"<<endl;
        }else{
            cout<<"lost it!!!"<<endl;
        }
    }
    cout<<"______IN___STATE_"<< FUNC_state <<"_____"<<endl;


    cout<<"___DIS___"<<DIS(*targetBallPlacement ,ball.Position)<<endl;


}