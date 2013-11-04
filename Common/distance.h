#pragma once

#include <math.h>
#include "Vector.h"

void calculate_dis_lut ( void );
float DIS ( const TVec3 a , const TVec3 b );
float DIS ( const TVec2 a , const TVec2 b );
float DISPOW ( TVec2  a , const TVec2 b );
float DIS ( const float x1 , const float y1 , const float x2 , const float y2 );
float DISL ( const TVec2 a , const TVec2 b );
float DISM ( const TVec2 a , const TVec2 b );
//float DIS ( float a , float b );
