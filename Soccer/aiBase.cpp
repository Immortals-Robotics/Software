#include "aiBase.h"

void aiBase::AddDebugPoint ( const TVec2& p , const CommonColor _color )
{
	if ( !debugDraw )
		return;
	Debug_Point* dbgPoint = AIDebug.add_point();
	dbgPoint -> set_x(p.X);
	dbgPoint -> set_y(p.Y);
	color * col = dbgPoint -> mutable_col();
	col -> set_r(CommonColorVal[_color][0]);
	col -> set_g(CommonColorVal[_color][1]);
	col -> set_b(CommonColorVal[_color][2]);
}

void aiBase::AddDebugLine ( const TVec2& p1 , const TVec2& p2 , const CommonColor _color )
{
	if ( !debugDraw )
		return;
	Debug_Line* dbgLine = AIDebug.add_line();
	dbgLine -> set_x1(p1.X);
	dbgLine -> set_y1(p1.Y);
	dbgLine -> set_x2(p2.X);
	dbgLine -> set_y2(p2.Y);
	color * col = dbgLine -> mutable_col();
	col -> set_r(CommonColorVal[_color][0]);
	col -> set_g(CommonColorVal[_color][1]);
	col -> set_b(CommonColorVal[_color][2]);
}

void aiBase::AddDebugRect ( const TVec2& p , const float w , const float h , const CommonColor _color )
{
	if ( !debugDraw )
		return;
	Debug_Rect* dbgRect = AIDebug.add_rect();
	dbgRect -> set_x(p.X);
	dbgRect -> set_y(p.Y);
	dbgRect -> set_w(w);
	dbgRect -> set_h(h);
	color * col = dbgRect -> mutable_col();
	col -> set_r(CommonColorVal[_color][0]);
	col -> set_g(CommonColorVal[_color][1]);
	col -> set_b(CommonColorVal[_color][2]);
}

void aiBase::AddDebugCircle ( const TVec2& p , const float r , const CommonColor _color )
{
	if ( !debugDraw )
		return;
	Debug_Circle* dbgCircle = AIDebug.add_circle();
	dbgCircle -> set_x(p.X);
	dbgCircle -> set_y(p.Y);
	dbgCircle -> set_r(r);
	color * col = dbgCircle -> mutable_col();
	col -> set_r(CommonColorVal[_color][0]);
	col -> set_g(CommonColorVal[_color][1]);
	col -> set_b(CommonColorVal[_color][2]);
}