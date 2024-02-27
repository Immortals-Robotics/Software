//
// Created by server on 2/13/18.
//

#ifndef CALIBENGINE_PID_CONTROLLER_H
#define CALIBENGINE_PID_CONTROLLER_H


class PID_Controller {
public:
    double iGain,pGain,dGain;
    double iState;
    double iMax,iMin;

    PID_Controller(double iGain = 0.32, double pGain = 25, double dGain = 0, double iMax = 328, double iMin = -328);
    double calcPI(double error);
};


#endif //CALIBENGINE_PID_CONTROLLER_H
