//
// Created by Ali Salehi on 18.06.18.
//

#include "Trajectory.h"
#include <float.h>

Trajectory Trajectory::MakeTrajectory(const RobotState &state, const TVec2 &a_acc, const float a_dec, const float a_dt)
{
    Trajectory trajectory;

    const float t0 = 0.f;

    const TVec2 p0 = state.Position;
    const TVec2 v0 = Vec2(state.velocity.x, state.velocity.y);
    const TVec2 a0 = a_acc;

    const float t1 = t0 + a_dt;

    trajectory.acc.t0 = t0;
    trajectory.acc.t1 = t1;
    trajectory.acc.p = p0;
    trajectory.acc.v = v0;
    trajectory.acc.a = a0;

    const TVec2 p1 = trajectory.acc.Evaluate(t1);
    const TVec2 v1 = trajectory.acc.EvaluateDerivative(t1);
    const TVec2 a1 = Normalize(v1) * (- a_dec);

    const float dec_dt = Magnitude(v1) / a_dec;
    const float t2 = t1 + dec_dt;

    trajectory.dec.t0 = t1;
    trajectory.dec.t1 = t2;
    trajectory.dec.p = p1;
    trajectory.dec.v = v1;
    trajectory.dec.a = a1;

    const TVec2 p2 = trajectory.dec.Evaluate(t2);
    const TVec2 v2 = Vec2(0.f, 0.f);
    const TVec2 a2 = Vec2(0.f, 0.f);

    trajectory.stopped.t0 = t2;
    trajectory.stopped.t1 = MAXFLOAT;
    trajectory.stopped.p = p2;
    trajectory.stopped.v = v2;
    trajectory.stopped.a = a2;

    return trajectory;
}

Trajectory Trajectory::MakeOpponentTrajectory(const RobotState &state, const float a_dec)
{
    Trajectory trajectory;

    const float t0 = -1.f;

    const TVec2 p0 = state.Position;
    const TVec2 v0 = Vec2(state.velocity.x, state.velocity.y);
    const TVec2 a0 = Vec2(0.f, 0.f);

    const float t1 = 0.f;

    trajectory.acc.t0 = t0;
    trajectory.acc.t1 = t1;
    trajectory.acc.p = p0;
    trajectory.acc.v = v0;
    trajectory.acc.a = a0;

    const TVec2 p1 = p0;
    const TVec2 v1 = v0;
    const TVec2 a1 = Normalize(v1) * (- a_dec);

    const float dec_dt = Magnitude(v1) / a_dec;
    const float t2 = t1 + dec_dt;

    trajectory.dec.t0 = t1;
    trajectory.dec.t1 = t2;
    trajectory.dec.p = p1;
    trajectory.dec.v = v1;
    trajectory.dec.a = a1;

    const TVec2 p2 = trajectory.dec.Evaluate(t2);
    const TVec2 v2 = Vec2(0.f, 0.f);
    const TVec2 a2 = Vec2(0.f, 0.f);

    trajectory.stopped.t0 = t2;
    trajectory.stopped.t1 = FLT_MAX;
    trajectory.stopped.p = p2;
    trajectory.stopped.v = v2;
    trajectory.stopped.a = a2;

    return trajectory;
}
