//
// Created by Ali Salehi on 18.06.18.
//

#ifndef IMMORTALSSSL_DSS_H
#define IMMORTALSSSL_DSS_H


#include "../../../Reality/WorldState.h"
#include "../Robot.h"
#include "../../../Common/Random.h"

class Dss
{
private:
    Random random;

    const float robot_r;
    const float cmd_dt;
    const float max_dec;
    const float max_dec_opp;

    const Robot *const own_robots;
    const RobotState *const opp_robots;

    TVec2 computed_motions[MAX_TEAM_ROBOTS];

    TVec2 GetAccFromMotion(const int robot_num, const TVec2 &motion);
    TVec2 GetMotionFromAcc(const int robot_num, const TVec2 &acc);

    bool OwnRobotsHaveCollision(const RobotState &state_a, const TVec2 &cmd_a,
                                const RobotState &state_b, const TVec2 &cmd_b) const;
    bool OppRobotsHaveCollision(const RobotState &state_own, const TVec2 &cmd_own,
                                const RobotState &state_opp) const;
    bool RobotHasStaticCollision(const RobotState &state, const TVec2 &cmd) const;

    bool IsAccSafe(const int robot_num, const TVec2 &cmd);
    TVec2 GetRandomAcceleration(const TVec2 &v, const float a_mag) const;
    static float ComputeError(const TVec2 &target, const TVec2 & current);

public:
    Dss(const Robot *const own_robots,
        const RobotState *const opp_robots,
        const float robot_r, const float cmd_dt,
        const float max_dec, const float max_dec_opp);

    void Reset( void );

    TVec2 ComputeSafeMotion(const int robot_num, const TVec2 &motion);
};


#endif //IMMORTALSSSL_DSS_H
