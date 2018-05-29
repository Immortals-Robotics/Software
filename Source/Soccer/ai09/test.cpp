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
    VELOCITY_PROFILE_TEST.max_spd = Vec2 ( 500.0f );
    VELOCITY_PROFILE_TEST.max_dec = Vec2 ( 2.0f );
    VELOCITY_PROFILE_TEST.max_acc = Vec2 ( 500.0f );
    VELOCITY_PROFILE_TEST.max_w_acc = 40.0f;
    VELOCITY_PROFILE_TEST.max_w_dec = 140.0f;

    if(time_dis.time()<4.0){
        OwnRobot[cmf].target.Angle = 90;
        Navigate2Point_2018(cmf,Vec2(-2000,-1500));
//        ERRTNavigate2Point(cmf,Vec2(-2000,-1500),0,80);
//        Navigate2Point(cmf,Vec2(-2000,-1000),0,80,&VELOCITY_PROFILE_TEST);
        //cout<<"first part"<<endl;
    }
    else if(time_dis.time()<8.0){
        OwnRobot[cmf].target.Angle = 90;
        Navigate2Point_2018(cmf,Vec2(-2000,1500));
//        ERRTNavigate2Point(cmf,Vec2(-2000,1500),0,80);
//        Navigate2Point(cmf,Vec2(-2000,1000),0,80,&VELOCITY_PROFILE_TEST);
    }
    else {
        time_dis.start();
    }
    //cout<<time_dis.time()<<endl;





}