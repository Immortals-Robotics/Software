
#include "line.h"

void Line::Construct ( float _x , float _y , float _m )
{
	x = _x;
	y = _y;

	m = _m;
}

void Line::Construct ( float _x1 , float _y1 , float _x2 , float _y2 )
{
	Construct ( _x1 , _y1 , (_y2-_y1)/(_x2-_x1) );
}