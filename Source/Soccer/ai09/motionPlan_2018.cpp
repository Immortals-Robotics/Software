//
// Created by dot_blue on 5/29/18.
//

#include "Robot.h"
#include "../../Common/ControlBlock.h"
#include "../../Common/PID_Controller.h"
#include "../../Common/distance.h"


double minimum_move_dist(double V0,double V1,double a_max){
    if(V0*V1 < 0) {
        cout << "Error: not the same" << endl;
        V0 = 0.0;
    }

    return (V1*V1 - V0*V0)/(2*a_max);
}

struct RobotMotionConfig{

};

TVec3 Robot::MotionPlan_2018 ( RobotState state , RobotState target , float speed , VelocityProfile * velocityProfile ){

    static median Vels(10);
    static double lastVel = 0.0;

    TVec2 deltaPos = target.Position - state.Position;
    TVec3 output;
    float path_ang = AngleWith(state.Position,target.Position);
    cout<<path_ang<<endl;

    double max_a = 1.2;
    double max_v = 120;
    double max_dec = -0.8;
    double distance = Magnitude(deltaPos);

    double tempVel,visionVel;

    if(State.seenState != Seen)
        cout<<"Oh my god!!!!"<<endl;
    if(State.seenState == Seen) {
        Vels.add_data(state.velocity.magnitude);
        cout<<"made it"<<endl;
    }

    visionVel = Vels.get_output()/50;
    if(distance < 40) {
        tempVel = lastVel + max_dec;
        cout<<"first PART!!!!!"<<endl;
    }
    else if(distance <= minimum_move_dist(visionVel,0.0,max_dec)*2.3){
        tempVel = lastVel + max_dec;
        cout<<"trying to stop!!!!!"<<endl;
    }
    else if(distance <= minimum_move_dist(visionVel,0.0,max_dec) + 300){
        tempVel = lastVel;
    }
    else
        tempVel = lastVel + max_a;
//    cout<<"dist is: "<<distance<<endl;



//    cout<<"VEL is:"<<Vels.get_output()/50<<endl;
//    Vels.print_all();


    if (tempVel > max_v)
        tempVel = max_v;

    if (tempVel < 0)
        tempVel = 0;

    lastVel = tempVel;

    output.X = tempVel * deltaPos.X/Magnitude(deltaPos);
    output.Y = tempVel * deltaPos.Y/Magnitude(deltaPos);
    return output;

}



