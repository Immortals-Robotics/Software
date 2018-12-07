

#include "ai09.h"




void ai09::our_place_ball_shoot(void) {

    GKHi(gk,true);
    DefMid(def,rw,lw,NULL,true);

    static float move_angle,temp_opp_ang;
    if( DIS(ball.Position,*targetBallPlacement)>100) {
        move_angle = AngleWith(*targetBallPlacement, ball.Position);
        temp_opp_ang = NormalizeAngle(move_angle + 180);
    }

    static float t_ang,t_opp_ang;
    static TVec2 t_pos;
    static VelocityProfile TEMP_VEL_PRO = BALL_PLACE_KHEYLI_SOOSKI;
    static Timer temp_time;

    cout<<"BEFORE: "<<dmf<<"_"<<mid2<<endl;
//    want_this_robot(attack);
//    want_this_robot(dmf);
    position_robots();
    cout<<"AFTER: "<<dmf<<"_"<<mid2<<endl;

    /*ERRTSetObstacles ( mid1 , false , true , true , true );
    AddCircle ( ball.Position.X , ball.Position.Y , 1010.0f );
    OwnRobot[mid1].face(ball.Position);
    ERRTNavigate2Point ( mid1 , PointOnConnectingLine(ball.Position, Vec2(side*field_width, 0), DIS(ball.Position, Vec2(side*field_width, 0))/3.0f) ,0 , 100,&VELOCITY_PROFILE_AROOM);*/

    ERRTSetObstacles ( rw , false , true , true , true );
    AddCircle ( ball.Position.X , ball.Position.Y , 1010.0f );
    OwnRobot[rw].face(ball.Position);
    ERRTNavigate2Point ( rw ,Vec2(0,-100) + PointOnConnectingLine(ball.Position, Vec2(side*field_width, 0), DIS(ball.Position, Vec2(side*field_width, 0))/3.0f) ,0 , 100,&VELOCITY_PROFILE_AROOM);

    ERRTSetObstacles ( lw , false , true , true , true );
    AddCircle ( ball.Position.X , ball.Position.Y , 1010.0f );
    OwnRobot[lw].face(ball.Position);
    ERRTNavigate2Point ( lw ,Vec2(0,100) + PointOnConnectingLine(ball.Position, Vec2(side*field_width, 0), DIS(ball.Position, Vec2(side*field_width, 0))/3.0f) ,0 , 100,&VELOCITY_PROFILE_AROOM);

    ERRTSetObstacles ( mid2 , false , true , true , true );
    AddCircle ( ball.Position.X , ball.Position.Y , 1010.0f );
    OwnRobot[mid2].face(Vec2(ball.Position.X,ball.Position.Y));
    ERRTNavigate2Point ( mid2 , CircleAroundPoint(Vec2(ball.Position.X,ball.Position.Y),NormalizeAngle(AngleWith(ball.Position , Vec2(side*field_width,0))),1090) ,0 , 100,&VELOCITY_PROFILE_AROOM);




    OwnRobot[attack].target.velocity.x = 0.0;
    OwnRobot[attack].target.velocity.y = 0.0;
    OwnRobot[dmf].target.velocity.x = 0.0;
    OwnRobot[dmf].target.velocity.y = 0.0;
//    clear_map ( );
    /*if(timer.time()>7 && FUNC_state < 2){//What the hell are you waiting for?!?!
        FUNC_state = 3;
        FUNC_CNT = 0;
    }*/
    if(FUNC_state == 0) {
        if(DIS(ball.Position,*targetBallPlacement) < 95){
            FUNC_state = 7;
            FUNC_CNT = 0;
            t_ang = 0;
            t_opp_ang = 180;
        }
        else if(DIS(ball.Position,*targetBallPlacement) < 1000) {
            FUNC_state = 3;
            FUNC_CNT = 0;
        }

//        OwnRobot[attack].target.Angle = temp_opp_ang;
        OwnRobot[dmf].target.Angle = move_angle;
//        ERRTSetObstacles(attack,0,0,1,1,0,0);
//        AddCircle(ball.Position.X, ball.Position.Y, 150.0f);
//        ERRTNavigate2Point(attack, CircleAroundPoint(ball.Position, move_angle, 250), 0, 40, &VELOCITY_PROFILE_AROOM);

        ERRTSetObstacles(dmf,0,0,1,1,0,0);
        AddCircle(ball.Position.X, ball.Position.Y, 150.0f);
        ERRTNavigate2Point(dmf, CircleAroundPoint(*targetBallPlacement, temp_opp_ang, 250), 0, 40, &VELOCITY_PROFILE_AROOM);

        clear_map ( );
        circle_ball_free(attack,move_angle,0,0,0.0);
        cout<<":::"<<OwnRobot[attack].State.velocity.magnitude<<endl<<OwnRobot[dmf].State.velocity.magnitude<<endl;
        if(OwnRobot[attack].State.velocity.magnitude < 20 &&
           OwnRobot[dmf].State.velocity.magnitude < 20){
            FUNC_CNT++;
        }
        if(FUNC_CNT >= 10 ){
            FUNC_state = 1;
            FUNC_CNT = 0;
        }

    }
    else if(FUNC_state == 1) {

        circle_ball_free(attack,move_angle,18,0,0.0);
        WaitForPass(dmf,false,&OwnRobot[attack].State.Position);
//        OwnRobot[dmf].target.Angle = 90;
        if(DIS(OwnRobot[attack].State.Position,ball.Position) > 400){
            FUNC_CNT++;
        }
        if(FUNC_CNT >= 20 ){
            FUNC_state = 2;
            FUNC_CNT = 0;
            temp_time.start();
        }

    }
    else if(FUNC_state == 2){

        WaitForPass(dmf,false,&OwnRobot[attack].State.Position);
//        OwnRobot[dmf].target.Angle = 90;

        if(temp_time.time() > 2){
            FUNC_CNT++;
        }
        if(FUNC_CNT >= 10 ){
            FUNC_state = 3;
            FUNC_CNT = 0;
            temp_time.start();
        }
    }
    else if(FUNC_state == 3){

        if(fabs(NormalizeAngle(OwnRobot[attack].target.Angle - move_angle)) > 90)
            swap(move_angle,temp_opp_ang);
        OwnRobot[attack].target.Angle = move_angle;
        OwnRobot[dmf].target.Angle = temp_opp_ang;
        ERRTSetObstacles(attack,0,0,1,1,0,0);
        AddCircle(ball.Position.X, ball.Position.Y, 150.0f);
        ERRTNavigate2Point(attack, CircleAroundPoint(ball.Position, temp_opp_ang, 250), 0, 40, &VELOCITY_PROFILE_AROOM);
        ERRTSetObstacles(dmf,0,0,1,1,0,0);
        AddCircle(ball.Position.X, ball.Position.Y, 150.0f);

        ERRTNavigate2Point(dmf, CircleAroundPoint(ball.Position, move_angle, 250), 0, 40, &VELOCITY_PROFILE_AROOM);

        if(DIS(CircleAroundPoint ( ball.Position , temp_opp_ang , 250 ),OwnRobot[attack].State.Position) < 100 &&
           DIS(CircleAroundPoint ( ball.Position , move_angle , 250 ),OwnRobot[dmf].State.Position) < 100 &&
               ball.velocity.magnitude < 10 ){

            FUNC_CNT++;
        }
        if(FUNC_CNT >= 10 ){
            FUNC_state = 4;
            FUNC_CNT = 0;
            t_pos = ball.Position;
        }
    }else if(FUNC_state == 4){
        if(fabs(NormalizeAngle(OwnRobot[attack].target.Angle - move_angle)) > 90)
            swap(move_angle,temp_opp_ang);

//        if(AngleWith(OwnRobot[attack].State.Position,OwnRobot[dmf].State.Position))

        OwnRobot[attack].target.Angle = move_angle;
        OwnRobot[dmf].target.Angle = temp_opp_ang;
        Navigate2Point(attack, CircleAroundPoint(t_pos, temp_opp_ang, 75), 0, 20, &BALL_PLACE_KHEYLI_SOOSKI);

        Navigate2Point(dmf, CircleAroundPoint(t_pos, move_angle, 75), 0, 20, &BALL_PLACE_KHEYLI_SOOSKI);

        bool ball_has_slipt = DIS((OwnRobot[attack].State.Position + OwnRobot[dmf].State.Position)/2,ball.Position) > 300;

        if(ball.seenState == Seen && ball_has_slipt){
            FUNC_state = 3;
            FUNC_CNT = 0;
        }

        if(DIS(CircleAroundPoint ( t_pos , temp_opp_ang , 75 ),OwnRobot[attack].State.Position) < 40 &&
           DIS(CircleAroundPoint ( t_pos , move_angle , 75 ),OwnRobot[dmf].State.Position) < 40){
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
        OwnRobot[attack].target.Angle = t_ang;
        OwnRobot[dmf].target.Angle = t_opp_ang;
        Navigate2Point(attack, CircleAroundPoint(*targetBallPlacement, t_opp_ang, 75), 0, 20, &BALL_PLACE_KHEYLI_SOOSKI);

        Navigate2Point(dmf, CircleAroundPoint(*targetBallPlacement, t_ang, 75), 0, 20, &BALL_PLACE_KHEYLI_SOOSKI);

        bool ball_has_slipt = DIS((OwnRobot[attack].State.Position + OwnRobot[dmf].State.Position)/2,ball.Position) > 300;

        if(ball.seenState == Seen && ball_has_slipt){
            FUNC_state = 3;
            FUNC_CNT = 0;

        }

        if(DIS(CircleAroundPoint ( *targetBallPlacement , t_opp_ang , 75 ),OwnRobot[attack].State.Position) < 40 &&
           DIS(CircleAroundPoint ( *targetBallPlacement , t_ang , 75 ),OwnRobot[dmf].State.Position) < 40){
            FUNC_CNT++;
        }
        if(FUNC_CNT >= 30 ){
            FUNC_state = 6;
            FUNC_CNT = 0;
            TEMP_VEL_PRO.max_spd = Vec2(300);
        }
    }
    else if(FUNC_state == 6){

        OwnRobot[attack].target.Angle = t_ang;
        OwnRobot[dmf].target.Angle = t_opp_ang;
        //OwnRobot[attack].face(ball.Position);
        Navigate2Point(attack, CircleAroundPoint(*targetBallPlacement, t_opp_ang, 250), 0, 20, &TEMP_VEL_PRO);
        //OwnRobot[dmf].face(ball.Position);
        Navigate2Point(dmf, CircleAroundPoint(*targetBallPlacement, t_ang, 250), 0, 20, &TEMP_VEL_PRO);



        if(DIS(CircleAroundPoint ( *targetBallPlacement , t_opp_ang , 250 ),OwnRobot[attack].State.Position) < 40 &&
           DIS(CircleAroundPoint ( *targetBallPlacement , t_ang , 250 ),OwnRobot[dmf].State.Position) < 40){
            FUNC_CNT++;
        }
        if(FUNC_CNT >= 30 ){
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
        OwnRobot[attack].face(ball.Position);//TODO test this
        ERRTSetObstacles(attack,1,1,1,1);
        ERRTNavigate2Point(attack, CircleAroundPoint(*targetBallPlacement, t_opp_ang, 550), 0, 20, &BALL_PLACE_KHEYLI_SOOSKI);
        OwnRobot[dmf].face(ball.Position);
        ERRTSetObstacles(dmf,1,1,1,1);
        ERRTNavigate2Point(dmf, CircleAroundPoint(*targetBallPlacement, t_ang, 550), 0, 20, &BALL_PLACE_KHEYLI_SOOSKI);

//        OwnRobot[attack].target.Angle = 0;
//        OwnRobot[dmf].target.Angle = 0;
//        t_ang = NormalizeAngle(OwnRobot[attack].State.Angle + 180);
//        Navigate2Point(attack, CircleAroundPoint(ball.Position, t_ang, 170), 0, 20, &BALL_PLACE_KHEYLI_SOOSKI);
//        t_ang = NormalizeAngle(OwnRobot[dmf].State.Angle + 180);
//        Navigate2Point(dmf, CircleAroundPoint(ball.Position, t_ang, 170), 0, 20, &BALL_PLACE_KHEYLI_SOOSKI);

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

void ai09::our_place_ball_shoot_V2(void) {

    GKHi(gk,true);
    DefMid(def,rw,lw,NULL,true);

    static float move_angle,temp_opp_ang;
    if( DIS(ball.Position,*targetBallPlacement)>100) {
        move_angle = AngleWith(*targetBallPlacement, ball.Position);
        temp_opp_ang = NormalizeAngle(move_angle + 180);
    }

    static float t_ang,t_opp_ang;
    static TVec2 t_pos;
    static VelocityProfile TEMP_VEL_PRO = BALL_PLACE_KHEYLI_SOOSKI;
    static Timer temp_time;

    static float outFieldAng = 0;
    static TVec2 last_state_ball_pos;

    cout<<"BEFORE: "<<dmf<<"_"<<mid2<<endl;
    //want_this_robot(attack);
    //want_this_robot(dmf);
    position_robots();
    cout<<"AFTER: "<<dmf<<"_"<<mid2<<endl;

    /*ERRTSetObstacles ( mid1 , false , true , true , true );
    AddCircle ( ball.Position.X , ball.Position.Y , 1010.0f );
    OwnRobot[mid1].face(ball.Position);
    ERRTNavigate2Point ( mid1 , PointOnConnectingLine(ball.Position, Vec2(side*field_width, 0), DIS(ball.Position, Vec2(side*field_width, 0))/3.0f) ,0 , 100,&VELOCITY_PROFILE_AROOM);*/

    ERRTSetObstacles ( rw , false , true , true , true );
    AddCircle ( ball.Position.X , ball.Position.Y , 1010.0f );
    OwnRobot[rw].face(ball.Position);
    ERRTNavigate2Point ( rw ,Vec2(0,-100) + PointOnConnectingLine(ball.Position, Vec2(side*field_width, 0), DIS(ball.Position, Vec2(side*field_width, 0))/3.0f) ,0 , 100,&VELOCITY_PROFILE_AROOM);

    ERRTSetObstacles ( lw , false , true , true , true );
    AddCircle ( ball.Position.X , ball.Position.Y , 1010.0f );
    OwnRobot[lw].face(ball.Position);
    ERRTNavigate2Point ( lw ,Vec2(0,100) + PointOnConnectingLine(ball.Position, Vec2(side*field_width, 0), DIS(ball.Position, Vec2(side*field_width, 0))/3.0f) ,0 , 100,&VELOCITY_PROFILE_AROOM);

    ERRTSetObstacles ( mid2 , false , true , true , true );
    AddCircle ( ball.Position.X , ball.Position.Y , 1010.0f );
    OwnRobot[mid2].face(Vec2(ball.Position.X,ball.Position.Y));
    ERRTNavigate2Point ( mid2 , CircleAroundPoint(Vec2(ball.Position.X,ball.Position.Y),NormalizeAngle(AngleWith(ball.Position , Vec2(side*field_width,0))),1090) ,0 , 100,&VELOCITY_PROFILE_AROOM);




    OwnRobot[attack].target.velocity.x = 0.0;
    OwnRobot[attack].target.velocity.y = 0.0;
    OwnRobot[dmf].target.velocity.x = 0.0;
    OwnRobot[dmf].target.velocity.y = 0.0;
//    clear_map ( );
    /*if(timer.time()>7 && FUNC_state < 2){//What the hell are you waiting for?!?!
        FUNC_state = 3;
        FUNC_CNT = 0;
    }*/
    if(FUNC_state == -2){
        circle_ball_free(attack,outFieldAng,24,0,0.0);

        if(DIS(last_state_ball_pos,ball.Position) > 400){
            FUNC_CNT++;
        }
        if(FUNC_CNT >= 20 ){
            FUNC_state = 3;//Back on track...
            FUNC_CNT = 0;
            temp_time.start();
        }
    }
    else if(FUNC_state == -1){
        if(outFieldAng == 0) {
            if (ball.Position.X > 0) {
                if (ball.Position.Y > 0) {
                    if(ball.Position.X > field_width)
                        outFieldAng = 155;
                    else//So fabs(ball.Position.Y) > field_height
                        outFieldAng = -65;
                } else {
                    if (ball.Position.X > field_width)
                        outFieldAng = -155;
                    else//So fabs(ball.Position.Y) > field_height
                        outFieldAng = 65;
                }
            } else {
                if (ball.Position.Y > 0) {
                    if(fabs(ball.Position.X) > field_width)
                        outFieldAng = 25;
                    else//So fabs(ball.Position.Y) > field_height
                        outFieldAng = -115;
                } else {
                    if(fabs(ball.Position.X) > field_width)
                        outFieldAng = -25;
                    else//So fabs(ball.Position.Y)  > field_height
                        outFieldAng = 115;
                }
            }
        }
        circle_ball_free(attack,outFieldAng,0,0,0.0);
        cout<<"outFieldAng: "<<outFieldAng<<endl;
        cout<<"OwnRobot[dmf].State.velocity.magnitude__"<<OwnRobot[dmf].State.velocity.magnitude<<endl;

        if(OwnRobot[attack].State.velocity.magnitude < 20) {
            FUNC_CNT++;
        }
        if(FUNC_CNT >= 10 ){
            FUNC_state = -2;
            FUNC_CNT = 0;
            last_state_ball_pos = ball.Position;
        }

    }
    else if(FUNC_state == 0) {
        if(DIS(ball.Position,*targetBallPlacement) < 95){
            FUNC_state = 7;
            FUNC_CNT = 0;
            t_ang = 0;
            t_opp_ang = 180;
        }
        else if(DIS(ball.Position,*targetBallPlacement) < 1000) {
            FUNC_state = 3;
            FUNC_CNT = 0;
        }
        else if(outOfField(ball.Position)){//Do a little shoot on the wall
            FUNC_state = -1;
            FUNC_CNT = 0;
            outFieldAng = 0;
        }

//        OwnRobot[attack].target.Angle = temp_opp_ang;
        OwnRobot[dmf].target.Angle = move_angle;
//        ERRTSetObstacles(attack,0,0,1,1,0,0);
//        AddCircle(ball.Position.X, ball.Position.Y, 150.0f);
//        ERRTNavigate2Point(attack, CircleAroundPoint(ball.Position, move_angle, 250), 0, 40, &VELOCITY_PROFILE_AROOM);

        ERRTSetObstacles(dmf,0,0,1,1,0,0);
        AddCircle(ball.Position.X, ball.Position.Y, 150.0f);
        ERRTNavigate2Point(dmf, CircleAroundPoint(*targetBallPlacement, temp_opp_ang, 250), 0, 40, &VELOCITY_PROFILE_AROOM);

        clear_map ( );
        circle_ball_free(attack,move_angle,0,0,0.0);
        cout<<":::"<<OwnRobot[attack].State.velocity.magnitude<<endl<<OwnRobot[dmf].State.velocity.magnitude<<endl;
        if(OwnRobot[attack].State.velocity.magnitude < 20 &&
           OwnRobot[dmf].State.velocity.magnitude < 20){
            FUNC_CNT++;
        }
        if(FUNC_CNT >= 10 ){
            FUNC_state = 1;
            FUNC_CNT = 0;
            last_state_ball_pos = ball.Position;
        }

    }
    else if(FUNC_state == 1) {

        circle_ball_free(attack,move_angle,18,0,0.0);
        WaitForPass(dmf,false,&OwnRobot[attack].State.Position);
//        OwnRobot[dmf].target.Angle = 90;
        if(DIS(last_state_ball_pos,ball.Position) > 400){//TODO added this TEST IT with the kicker on (already tested without the kicker)
            FUNC_CNT++;
        }
        if(DIS(OwnRobot[attack].State.Position,ball.Position) > 400){
            FUNC_CNT++;
        }
        if(FUNC_CNT >= 20 ){
            FUNC_state = 2;
            FUNC_CNT = 0;
            temp_time.start();
        }

    }
    else if(FUNC_state == 2){

        WaitForPass(dmf,false,&OwnRobot[attack].State.Position);
//        OwnRobot[dmf].target.Angle = 90;

        if(temp_time.time() > 2){
            FUNC_CNT++;
        }
        if(FUNC_CNT >= 10 ){
            FUNC_state = 3;
            FUNC_CNT = 0;
            temp_time.start();
        }
    }
    else if(FUNC_state == 3){

        if(fabs(NormalizeAngle(OwnRobot[attack].target.Angle - move_angle)) > 90)
            swap(move_angle,temp_opp_ang);
        OwnRobot[attack].target.Angle = move_angle;
        OwnRobot[dmf].target.Angle = temp_opp_ang;
        ERRTSetObstacles(attack,0,0,1,1,0,0);
        AddCircle(ball.Position.X, ball.Position.Y, 150.0f);
        ERRTNavigate2Point(attack, CircleAroundPoint(ball.Position, temp_opp_ang, 250), 0, 40, &VELOCITY_PROFILE_AROOM);
        ERRTSetObstacles(dmf,0,0,1,1,0,0);
        AddCircle(ball.Position.X, ball.Position.Y, 150.0f);

        ERRTNavigate2Point(dmf, CircleAroundPoint(ball.Position, move_angle, 250), 0, 40, &VELOCITY_PROFILE_AROOM);

        if(DIS(CircleAroundPoint ( ball.Position , temp_opp_ang , 250 ),OwnRobot[attack].State.Position) < 100 &&
           DIS(CircleAroundPoint ( ball.Position , move_angle , 250 ),OwnRobot[dmf].State.Position) < 100 &&
           ball.velocity.magnitude < 10 ){

            FUNC_CNT++;
        }
        if(FUNC_CNT >= 10 ){
            FUNC_state = 4;
            FUNC_CNT = 0;
            t_pos = ball.Position;
        }
    }else if(FUNC_state == 4){
        if(fabs(NormalizeAngle(OwnRobot[attack].target.Angle - move_angle)) > 90)
            swap(move_angle,temp_opp_ang);

//        if(AngleWith(OwnRobot[attack].State.Position,OwnRobot[dmf].State.Position))

        OwnRobot[attack].target.Angle = move_angle;
        OwnRobot[dmf].target.Angle = temp_opp_ang;
        Navigate2Point(attack, CircleAroundPoint(t_pos, temp_opp_ang, 75), 0, 20, &BALL_PLACE_KHEYLI_SOOSKI);

        Navigate2Point(dmf, CircleAroundPoint(t_pos, move_angle, 75), 0, 20, &BALL_PLACE_KHEYLI_SOOSKI);

        bool ball_has_slipt = DIS((OwnRobot[attack].State.Position + OwnRobot[dmf].State.Position)/2,ball.Position) > 300;

        if(ball.seenState == Seen && ball_has_slipt){
            FUNC_state = 3;
            FUNC_CNT = 0;
        }

        if(DIS(CircleAroundPoint ( t_pos , temp_opp_ang , 75 ),OwnRobot[attack].State.Position) < 40 &&
           DIS(CircleAroundPoint ( t_pos , move_angle , 75 ),OwnRobot[dmf].State.Position) < 40){
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
        OwnRobot[attack].target.Angle = t_ang;
        OwnRobot[dmf].target.Angle = t_opp_ang;
        Navigate2Point(attack, CircleAroundPoint(*targetBallPlacement, t_opp_ang, 75), 0, 20, &BALL_PLACE_KHEYLI_SOOSKI);

        Navigate2Point(dmf, CircleAroundPoint(*targetBallPlacement, t_ang, 75), 0, 20, &BALL_PLACE_KHEYLI_SOOSKI);

        bool ball_has_slipt = DIS((OwnRobot[attack].State.Position + OwnRobot[dmf].State.Position)/2,ball.Position) > 300;

        if(ball.seenState == Seen && ball_has_slipt){
            FUNC_state = 3;
            FUNC_CNT = 0;

        }

        if(DIS(CircleAroundPoint ( *targetBallPlacement , t_opp_ang , 75 ),OwnRobot[attack].State.Position) < 40 &&
           DIS(CircleAroundPoint ( *targetBallPlacement , t_ang , 75 ),OwnRobot[dmf].State.Position) < 40){
            FUNC_CNT++;
        }
        if(FUNC_CNT >= 30 ){
            FUNC_state = 6;
            FUNC_CNT = 0;
            TEMP_VEL_PRO.max_spd = Vec2(300);
        }
    }
    else if(FUNC_state == 6){

        OwnRobot[attack].target.Angle = t_ang;
        OwnRobot[dmf].target.Angle = t_opp_ang;
        //OwnRobot[attack].face(ball.Position);
        Navigate2Point(attack, CircleAroundPoint(*targetBallPlacement, t_opp_ang, 250), 0, 20, &TEMP_VEL_PRO);
        //OwnRobot[dmf].face(ball.Position);
        Navigate2Point(dmf, CircleAroundPoint(*targetBallPlacement, t_ang, 250), 0, 20, &TEMP_VEL_PRO);



        if(DIS(CircleAroundPoint ( *targetBallPlacement , t_opp_ang , 250 ),OwnRobot[attack].State.Position) < 40 &&
           DIS(CircleAroundPoint ( *targetBallPlacement , t_ang , 250 ),OwnRobot[dmf].State.Position) < 40){
            FUNC_CNT++;
        }
        if(FUNC_CNT >= 30 ){
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
        OwnRobot[attack].face(ball.Position);//TODO test this
        ERRTSetObstacles(attack,1,1,1,1);
        ERRTNavigate2Point(attack, CircleAroundPoint(*targetBallPlacement, t_opp_ang, 550), 0, 20, &BALL_PLACE_KHEYLI_SOOSKI);
        OwnRobot[dmf].face(ball.Position);
        ERRTSetObstacles(dmf,1,1,1,1);
        ERRTNavigate2Point(dmf, CircleAroundPoint(*targetBallPlacement, t_ang, 550), 0, 20, &BALL_PLACE_KHEYLI_SOOSKI);

//        OwnRobot[attack].target.Angle = 0;
//        OwnRobot[dmf].target.Angle = 0;
//        t_ang = NormalizeAngle(OwnRobot[attack].State.Angle + 180);
//        Navigate2Point(attack, CircleAroundPoint(ball.Position, t_ang, 170), 0, 20, &BALL_PLACE_KHEYLI_SOOSKI);
//        t_ang = NormalizeAngle(OwnRobot[dmf].State.Angle + 180);
//        Navigate2Point(dmf, CircleAroundPoint(ball.Position, t_ang, 170), 0, 20, &BALL_PLACE_KHEYLI_SOOSKI);

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
    cout<<"___BALL__POS_XY__"<<ball.Position.X<<'_'<<ball.Position.Y<<endl;
    cout<<"___TARGET__POS_XY__"<<targetBallPlacement->X<<'_'<<targetBallPlacement->Y<<endl;
    cout<<"__OUT__"<<outOfField(ball.Position)<<endl;


}

void ai09::our_place_ball_shoot_taki(void) {

    GKHi(gk,true);
    DefMid(def,rw,lw,NULL,true);

    static float move_angle,opp_ang;
    if( DIS(ball.Position,*targetBallPlacement)>150) {
        move_angle = AngleWith(*targetBallPlacement, ball.Position);
        opp_ang = NormalizeAngle(move_angle + 180);
    }

    static float t_ang,t_opp_ang;
    static TVec2 t_pos;
    static VelocityProfile TEMP_VEL_PRO = BALL_PLACE_KHEYLI_SOOSKI;
    static Timer temp_time;

    cout<<"BEFORE: "<<dmf<<"_"<<mid2<<endl;
    want_this_robot(attack);
    want_this_robot(dmf);
    position_robots();
    cout<<"AFTER: "<<dmf<<"_"<<mid2<<endl;

    ERRTSetObstacles ( dmf , false , true , true , true );
    AddCircle ( ball.Position.X , ball.Position.Y , 1010.0f );
    OwnRobot[dmf].face(ball.Position);
    ERRTNavigate2Point ( dmf , PointOnConnectingLine(ball.Position, Vec2(side*field_width, 0), DIS(ball.Position, Vec2(side*field_width, 0))/3.0f) ,0 , 100,&VELOCITY_PROFILE_AROOM);

    ERRTSetObstacles ( rw , false , true , true , true );
    AddCircle ( ball.Position.X , ball.Position.Y , 1010.0f );
    OwnRobot[rw].face(ball.Position);
    ERRTNavigate2Point ( rw ,Vec2(0,-100) + PointOnConnectingLine(ball.Position, Vec2(side*field_width, 0), DIS(ball.Position, Vec2(side*field_width, 0))/3.0f) ,0 , 100,&VELOCITY_PROFILE_AROOM);

    ERRTSetObstacles ( lw , false , true , true , true );
    AddCircle ( ball.Position.X , ball.Position.Y , 1010.0f );
    OwnRobot[lw].face(ball.Position);
    ERRTNavigate2Point ( lw ,Vec2(0,100) + PointOnConnectingLine(ball.Position, Vec2(side*field_width, 0), DIS(ball.Position, Vec2(side*field_width, 0))/3.0f) ,0 , 100,&VELOCITY_PROFILE_AROOM);

    ERRTSetObstacles ( mid2 , false , true , true , true );
    AddCircle ( ball.Position.X , ball.Position.Y , 1010.0f );
    OwnRobot[mid2].face(Vec2(ball.Position.X,ball.Position.Y));
    ERRTNavigate2Point ( mid2 , CircleAroundPoint(Vec2(ball.Position.X,ball.Position.Y),NormalizeAngle(AngleWith(ball.Position , Vec2(side*field_width,0))),1090) ,0 , 100,&VELOCITY_PROFILE_AROOM);




    OwnRobot[attack].target.velocity.x = 0.0;
    OwnRobot[attack].target.velocity.y = 0.0;
    OwnRobot[dmf].target.velocity.x = 0.0;
    OwnRobot[dmf].target.velocity.y = 0.0;
//    clear_map ( );

    if(FUNC_state == 0) {
        if(DIS(ball.Position,*targetBallPlacement) < 95){
            FUNC_state = 7;
            FUNC_CNT = 0;
            t_ang = 0;
            t_opp_ang = 180;
        }
        else if(DIS(ball.Position,*targetBallPlacement) < 1000) {
            FUNC_state = 3;
            FUNC_CNT = 0;
        }

//        OwnRobot[attack].target.Angle = temp_opp_ang;
        OwnRobot[dmf].target.Angle = move_angle;
//        ERRTSetObstacles(attack,0,0,1,1,0,0);
//        AddCircle(ball.Position.X, ball.Position.Y, 150.0f);
//        ERRTNavigate2Point(attack, CircleAroundPoint(ball.Position, move_angle, 250), 0, 40, &VELOCITY_PROFILE_AROOM);

        ERRTSetObstacles(dmf,0,0,1,1,0,0);
        AddCircle(ball.Position.X, ball.Position.Y, 150.0f);
        ERRTNavigate2Point(dmf, CircleAroundPoint(*targetBallPlacement, opp_ang, 250), 0, 40, &VELOCITY_PROFILE_AROOM);

        clear_map ( );
        circle_ball_free(attack,move_angle,0,0,0.0);
        cout<<":::"<<OwnRobot[attack].State.velocity.magnitude<<endl<<OwnRobot[dmf].State.velocity.magnitude<<endl;
        if(OwnRobot[attack].State.velocity.magnitude < 20 &&
           OwnRobot[dmf].State.velocity.magnitude < 20){
            FUNC_CNT++;
        }
        if(FUNC_CNT >= 10 ){
            FUNC_state = 1;
            FUNC_CNT = 0;
        }

    }
    else if(FUNC_state == 1) {

        circle_ball_free(attack,move_angle,60,0,0.0);
        WaitForPass(dmf,false,&OwnRobot[attack].State.Position);
//        OwnRobot[dmf].target.Angle = 90;
        if(DIS(OwnRobot[attack].State.Position,ball.Position) > 400){
            FUNC_CNT++;
        }
        if(FUNC_CNT >= 20 ){
            FUNC_state = 2;
            FUNC_CNT = 0;
            temp_time.start();
        }

    }
    else if(FUNC_state == 2){

        WaitForPass(dmf,false,&OwnRobot[attack].State.Position);
//        OwnRobot[dmf].target.Angle = 90;

        if(temp_time.time() > 2){
            FUNC_CNT++;
        }
        if(FUNC_CNT >= 10 ){
            FUNC_state = 3;
            FUNC_CNT = 0;
            temp_time.start();
        }
    }
    else if(FUNC_state == 3){

//        OwnRobot[attack].target.Angle = move_angle;
//        ERRTSetObstacles(attack,0,0,1,1,0,0);
//        AddCircle(ball.Position.X, ball.Position.Y, 150.0f);
        VelocityProfile temp_vel = BALL_PLACE_KHEYLI_SOOSKI;
        temp_vel.max_spd = Vec2(10.0);
        if(DIS(ball.Position,*targetBallPlacement) < 350)
            temp_vel.max_spd = Vec2(3.0);
//        circle_ball_free_V2(attack,move_angle,1,0,0,temp_vel);

        OwnRobot[attack].target.Angle = move_angle;
        OwnRobot[dmf].target.Angle = opp_ang;
        Navigate2Point(attack, CircleAroundPoint(t_pos, opp_ang, 75), 0, 20, &temp_vel);

        Navigate2Point(dmf, CircleAroundPoint(*targetBallPlacement, move_angle, 75), 0, 20, &BALL_PLACE_KHEYLI_SOOSKI);


//        float angle_to_face = AngleWith(*targetBallPlacement,OwnRobot[attack].State.Position);
//        OwnRobot[dmf].target.Angle = angle_to_face;
//        ERRTSetObstacles(dmf,0,0,1,1,0,0);
////        TEMP_VEL_PRO.max_spd
//        ERRTNavigate2Point(dmf, CircleAroundPoint(*targetBallPlacement, angle_to_face + 180, 100), 0, 5, &BALL_PLACE_KHEYLI_SOOSKI);

        if(DIS(ball.Position,*targetBallPlacement) < 100 && ball.velocity.magnitude < 10){

            FUNC_CNT++;
        }
        if(FUNC_CNT >= 10 ){
            FUNC_state = 4;
            FUNC_CNT = 0;
        }
    }
    else{
//        OwnRobot[attack].face(ball.Position);
//        Navigate2Point(attack,OwnRobot[attack].State.Position,false,0,&BALL_PLACE_KHEYLI_SOOSKI);
        OwnRobot[attack].face(ball.Position);//TODO test this
        ERRTSetObstacles(attack,1,1,1,1);
        ERRTNavigate2Point(attack, CircleAroundPoint(*targetBallPlacement, t_opp_ang, 550), 0, 20, &BALL_PLACE_KHEYLI_SOOSKI);
        OwnRobot[dmf].face(ball.Position);
        ERRTSetObstacles(dmf,1,1,1,1);
        ERRTNavigate2Point(dmf, CircleAroundPoint(*targetBallPlacement, t_ang, 550), 0, 20, &BALL_PLACE_KHEYLI_SOOSKI);

//        OwnRobot[attack].target.Angle = 0;
//        OwnRobot[dmf].target.Angle = 0;
//        t_ang = NormalizeAngle(OwnRobot[attack].State.Angle + 180);
//        Navigate2Point(attack, CircleAroundPoint(ball.Position, t_ang, 170), 0, 20, &BALL_PLACE_KHEYLI_SOOSKI);
//        t_ang = NormalizeAngle(OwnRobot[dmf].State.Angle + 180);
//        Navigate2Point(dmf, CircleAroundPoint(ball.Position, t_ang, 170), 0, 20, &BALL_PLACE_KHEYLI_SOOSKI);

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

