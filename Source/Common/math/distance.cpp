#include "distance.h"

float DIS ( const TVec3 a , const TVec3 b )
{
    float dx = a.X - b.X;
    float dy = a.Y - b.Y;
    float dz = a.Z - b.Z;
    return (float) sqrt (( dx ) * ( dx ) +
					  ( dy ) * ( dy ) +
					  ( dz ) * ( dz ) );
}

float DIS ( const TVec2 a , const TVec2 b )
{
    float dx = a.X - b.X;
    float dy = a.Y - b.Y;
    return (float) sqrt (dx * dx + dy * dy );
}

float DIS ( const float x1 , const float y1 , const float x2 , const float y2 )
{
    float dx = x1 - x2;
    float dy = y1 - y2;
    return (float) sqrt (dx * dx + dy * dy );
}