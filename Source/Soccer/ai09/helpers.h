#pragma once

#include "../../Common/Vector.h"
//#include "WorldState.h"

#ifndef max
float max(float a, float b);
#endif
#ifndef min
float min(float a, float b);
#endif

/*#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif*/

int sgn(float num);
bool sgnBool(float num);

float AngleWith(TVec2 a,TVec2 b);

float dis(float x1,float y1,float x2,float y2);

float NormalizeAngle(float teta);

TVec2 PointOnConnectingLine(TVec2 FirstPoint,TVec2 SecondPoint,float distance);

TVec2 CircleAroundPoint(TVec2 point,float angle,float radius);