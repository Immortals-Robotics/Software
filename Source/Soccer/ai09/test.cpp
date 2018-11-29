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
    VELOCITY_PROFILE_TEST.max_dec = Vec2 ( 1.8f );
    VELOCITY_PROFILE_TEST.max_acc = Vec2 ( 2.3f );

    if(time_dis.time()<4.0){
        OwnRobot[cmf].target.Angle = 90;
//        Navigate2Point_2018(cmf,Vec2(-2000,-2000));
//        ERRTNavigate2Point(cmf,Vec2(-2000,-1500),0,80);
        Navigate2Point(cmf,Vec2(-1000,700),0,20,&VELOCITY_PROFILE_TEST,false);

        //cout<<"first part"<<endl;
    }
    else if(time_dis.time()<8.0){
        OwnRobot[cmf].target.Angle = 90;
//        Navigate2Point_2018(cmf,Vec2(-2000,2000));
//        ERRTNavigate2Point(cmf,Vec2(-2000,1500),0,80);

        Navigate2Point(cmf,Vec2(-3500,2300),0,20,&VELOCITY_PROFILE_TEST,false);

    }
    else {
        time_dis.start();
    }
    //cout<<time_dis.time()<<endl;
    plot->BLUE_pushData(OwnRobot[cmf].target.velocity.magnitude);


    plot->send_data();





}