//
// Created by server on 2/13/18.
//

#include "PID_Controller.h"

PID_Controller::PID_Controller(double iGain, double pGain, double dGain, double iMax, double iMin) :
                             iGain(iGain),pGain(pGain),dGain(dGain),iMax(iMax),iMin(iMin),iState(0.0) {

}

double PID_Controller::calcPI(double error) {
    double pTerm = pGain * error;
    double iTerm;

    double iState = iState + error;

    if(iState>iMax)
        iState=iMax;
    if(iState<iMin)
        iState=iMin;
    iTerm =iGain * iState;


    return pTerm + iTerm;
}
