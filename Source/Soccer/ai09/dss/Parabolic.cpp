//
// Created by Ali Salehi on 18.06.18.
//

#include "Parabolic.h"
#include "../helpers.h"
#include "../../../Common/distance.h"
#include "../errt/obstacle.h"

bool Parabolic::HaveOverlap(const Parabolic &a, const Parabolic &b, const float r)
{
    const float t0 = min(a.t0, b.t0);
    const float t1 = min(a.t1, b.t1);

    if (t1 <= t0)
    {
        return false;
    }

    float check_t[6];
    int check_t_count = 0;

    check_t[check_t_count++] = t0;
    check_t[check_t_count++] = t1;
    check_t[check_t_count++] = (t0 + t1) / 2.f;

    for (int t_idx = 0; t_idx < check_t_count; ++t_idx)
    {
        const float t = check_t[t_idx];
        if (HasCollision(a, b, r, t))
        {
            return true;
        }
    }

    return false;
}

bool Parabolic::HasStaticOverlap(const Parabolic &a)
{
    const float t0 = a.t0;
    const float t1 = a.t1;

    if (t1 <= t0)
    {
        return false;
    }

    float check_t[6];
    int check_t_count = 0;

    check_t[check_t_count++] = t0;
    check_t[check_t_count++] = t1;
    check_t[check_t_count++] = (t0 + t1) / 2.f;

    for (int t_idx = 0; t_idx < check_t_count; ++t_idx)
    {
        const float t = check_t[t_idx];
        const TVec2 p = a.Evaluate(t);
        if (IsInObstacle(p))
        {
            return true;
        }
    }

    return false;
}

bool Parabolic::HasCollision(const Parabolic &a, const Parabolic &b, const float r, const float t)
{
    const float t0 = min(a.t0, b.t0);
    const float t1 = min(a.t1, b.t1);

    if (t1 <= t0 ||
        t < t0 ||
        t > t1)
    {
        return false;
    }

    const float dis = Distance(a, b, t);
    return dis <= r;
}

float Parabolic::Distance(const Parabolic &a, const Parabolic &b, const float t)
{
    const TVec2 p_a = a.Evaluate(t);
    const TVec2 p_b = b.Evaluate(t);
    return DIS(p_a, p_b);
}

TVec2 Parabolic::Evaluate(const float t) const
{
    if (Magnitude(a) < 1e-10 && Magnitude(v) < 1e-10)
    {
        return p;
    }

    const float dt = t - t0;
    const float x = p.X + (v.X * dt) + (a.X * (dt * dt * .5f));
    const float y = p.Y + (v.Y * dt) + (a.Y * (dt * dt * .5f));
    return Vec2(x, y);
}

TVec2 Parabolic::EvaluateDerivative(const float t) const
{
    if (Magnitude(a) < 1e-10)
    {
        return v;
    }

    const float dt = t - t0;
    const float x = v.X + (a.X * dt);
    const float y = v.Y + (a.Y * dt);
    return Vec2(x, y);
}

