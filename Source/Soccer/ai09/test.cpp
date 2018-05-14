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

    if(time_dis.time()<4.0){
        OwnRobot[cmf].target.Angle = 90;
        Navigate2Point(cmf,Vec2(-2000,-1000),0,80,&VELOCITY_PROFILE_MAMOOLI);
        cout<<"first part"<<endl;
    }
    else if(time_dis.time()<8.0){
        OwnRobot[cmf].target.Angle = 90;
        Navigate2Point(cmf,Vec2(-2000,1000),0,80,&VELOCITY_PROFILE_MAMOOLI);
    }
    else {
        time_dis.start();
    }
    cout<<time_dis.time()<<endl;





}