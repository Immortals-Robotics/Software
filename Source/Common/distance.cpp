#include "distance.h"
#include <cstdlib>

float dis_lut [610][410];

void calculate_dis_lut ( void )
{
	for ( int i = 0 ; i < 610 ; i ++ )
		for ( int j = 0 ; j < 410 ; j ++ )
			dis_lut[i][j] = sqrt((float)( i * i + j * j ) );
	//int a;
}

float dx , dy , dz;

float DIS ( const TVec3 a , const TVec3 b )
{
    dx = a.X - b.X;
    dy = a.Y - b.Y;
    dz = a.Z - b.Z;
    return sqrt ( ( dx ) * ( dx ) +
                  ( dy ) * ( dy ) +
                  ( dz ) * ( dz ) );
}

float DIS ( const TVec2 a , const TVec2 b )
{
    dx = a.X - b.X;
    dy = a.Y - b.Y;
    return sqrt ( dx * dx + dy * dy );
}

float DISPOW ( TVec2  a , const TVec2 b )
{
    dx = a.X - b.X;
    dy = a.Y - b.Y;
        return ( dx * dx + dy * dy );
}

float DISM ( const TVec2 a , const TVec2 b )
{
    dx = fabs ( a.X - b.X );
    dy = fabs ( a.Y - b.Y );
        return dx + dy;
}

float DIS ( const float x1 , const float y1 , const float x2 , const float y2 )
{
    dx = x1 - x2;
    dy = y1 - y2;
    return sqrt ( dx * dx + dy * dy );
}

float DISL ( const TVec2 a , const TVec2 b )
{
	return dis_lut[(int)fabs(a.X-b.X)][(int)fabs(a.Y-b.Y)];
}

/*float DIS ( float & a , float & b )
{
        return sqrt ( a * a + b * b );
}*/
