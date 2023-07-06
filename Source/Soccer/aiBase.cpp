#include "aiBase.h"

void aiBase::AddDebugPoint ( const TVec2& p , const CommonColor _color )
{
	debug().drawPoint(p);
}

void aiBase::AddDebugLine ( const TVec2& p1 , const TVec2& p2 , const CommonColor _color )
{
	debug().drawLineSegment(p1, p2);
}

void aiBase::AddDebugRect ( const TVec2& p , const float w , const float h , const CommonColor _color )
{
	debug().drawRect(p, w, h);
}

void aiBase::AddDebugCircle ( const TVec2& p , const float r , const CommonColor _color )
{
	debug().drawCircle(p, r);
}