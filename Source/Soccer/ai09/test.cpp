//
// Created by dot_blue on 5/9/18.
//

#include "ai09.h"


void ai09::my_test() {
    static Timer time_dis;
    static bool first_call=true;
    if(first_call) {
        time_dis.start();
        first_call = false;
    }
    VelocityProfile VELOCITY_PROFILE_TEST;
//    VELOCITY_PROFILE_TEST.max_spd = Vec2 ( 3000.0f );
//    VELOCITY_PROFILE_TEST.max_dec = Vec2 ( 2f );
//    VELOCITY_PROFILE_TEST.max_acc = Vec2 ( 1500.0f );
//    VELOCITY_PROFILE_TEST.max_w_acc = 40.0f;
//    VELOCITY_PROFILE_TEST.max_w_dec = 140.0f;
    VELOCITY_PROFILE_TEST = VELOCITY_PROFILE_KHARAKI;
    VELOCITY_PROFILE_TEST.max_spd = Vec2 ( 150.0f );
    VELOCITY_PROFILE_TEST.max_dec = Vec2 ( 1.5f );//1.8
    VELOCITY_PROFILE_TEST.max_acc = Vec2 ( 1.5f );//2.3

    if(time_dis.time()<6.0){
        OwnRobot[cmf].target.Angle = 90;
//        Navigate2Point_2018(cmf,Vec2(-2000,-2000));
//        ERRTNavigate2Point(cmf,Vec2(-2000,-1500),0,80);
        Navigate2Point(cmf,Vec2(-1000,700),0,30,&VELOCITY_PROFILE_TEST,false);

        //cout<<"first part"<<endl;
    }
    else if(time_dis.time()<12.0){
        OwnRobot[cmf].target.Angle = 90;
//        Navigate2Point_2018(cmf,Vec2(-2000,2000));
//        ERRTNavigate2Point(cmf,Vec2(-2000,1500),0,80);

        Navigate2Point(cmf,Vec2(-3500,2300),0,30,&VELOCITY_PROFILE_TEST,false);

    }
    else {
        time_dis.start();
    }
    static double last_PosX = 0.0;
    static double integral_PosFilterX = 0.0;
    static double integral_PosRawX = 0.0;
    static int resetCounter = 0;

    if (resetCounter == 10){
        integral_PosRawX = integral_PosFilterX;

        resetCounter = -500;
    }
    resetCounter++;

    //cout<<time_dis.time()<<endl;
//    integral_PosRawX += (OwnRobot[cmf].State.Position.X - last_PosX);
    plot->BLUE_pushData(OwnRobot[cmf].State.velocity.x);

//    integral_PosFilterX += OwnRobot[cmf].State.velocity.x*0.9*worldState->delta_t_capture;
    plot->GREEN_pushData(OwnRobot[cmf].target.velocity.x * 48);

    cout<<"ALPHA-> "<<(OwnRobot[cmf].State.velocity.x/OwnRobot[cmf].target.velocity.x)<<endl;

//    cout<<"deltaX/time: "<<(OwnRobot[cmf].State.Position.X - last_PosX)/worldState->delta_t_capture<<endl;


    last_PosX = OwnRobot[cmf].State.Position.X;


    plot->send_data();





}