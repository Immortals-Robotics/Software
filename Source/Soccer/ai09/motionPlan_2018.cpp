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

TVec3 Robot::MotionPlan_2018 ( RobotState state , RobotState target , float speed , VelocityProfile * velocityProfile ){

    static PID_Controller Pos(0.02,3, 0, 328, -328);
    static double lastVel = 0.0;

    TVec2 deltaPos = target.Position - state.Position;
    TVec3 output;

    if(Magnitude(deltaPos)< 100) {
        lastVel *= 0.5;
        output.X = lastVel * deltaPos.X/Magnitude(deltaPos);
        output.Y = lastVel * deltaPos.Y/Magnitude(deltaPos);
        return output;
    }
//    Xvels.add_data(State.velocity.x);
//    Yvels.add_data(State.velocity.y);
//    cout<<"X is:"<<Xvels.get_output()<<endl
//        <<"Y is:"<<Yvels.get_output()<<endl;
    if(State.seenState != Seen)
        cout<<"Oh my god!!!!"<<endl;

    double tempVel = Pos.calcPI(DIS(state.Position,target.Position));

    if (tempVel - lastVel > 3)//SPEED UP
        tempVel = tempVel + 3;
    else if (tempVel - lastVel < -1)//BREAK
        tempVel = tempVel - 1;

    if (tempVel > 30)
        tempVel = 30;

    lastVel = tempVel;

    output.X = tempVel * deltaPos.X/Magnitude(deltaPos);
    output.Y = tempVel * deltaPos.Y/Magnitude(deltaPos);
    return output;

}



