//
// Created by Ali Salehi on 18.06.18.
//

#ifndef IMMORTALSSSL_PARABOLIC_H
#define IMMORTALSSSL_PARABOLIC_H

#include "../../../Common/Vector.h"

class Parabolic
{
private:
    static bool HasCollision(const Parabolic &a, const Parabolic &b, const float r, const float t);
    static float Distance(const Parabolic &a, const Parabolic &b, const float t);

public:
    TVec2 a;
    TVec2 v;
    TVec2 p;

    float t0;
    float t1;

    static bool HaveOverlap(const Parabolic &a, const Parabolic &b, const float r);
    static bool HasStaticOverlap(const Parabolic &a);

    TVec2 Evaluate(const float t) const;
    TVec2 EvaluateDerivative(const float t) const;
};


#endif //IMMORTALSSSL_PARABOLIC_H
