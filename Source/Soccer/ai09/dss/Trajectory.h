//
// Created by Ali Salehi on 18.06.18.
//

#ifndef IMMORTALSSSL_TRAJECTORY_H
#define IMMORTALSSSL_TRAJECTORY_H

#include "Parabolic.h"
#include "../../../Reality/WorldState.h"

class Trajectory
{
public:
    Parabolic acc;
    Parabolic dec;
    Parabolic stopped;

    static Trajectory MakeTrajectory(const RobotState &state, const TVec2 &a_acc, const float a_dec, const float a_dt);
    static Trajectory MakeOpponentTrajectory(const RobotState &state, const float a_dec);
};


#endif //IMMORTALSSSL_TRAJECTORY_H
