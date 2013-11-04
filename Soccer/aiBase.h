#pragma once

#include "WorldState.h"
#include "GameSetting.h"
#include "aidebug.pb.h"
#include "common_colors.h"

class aiBase
{
	protected:
	bool debugDraw;
	void AddDebugPoint ( const TVec2& p , const CommonColor _color = White );
	void AddDebugLine ( const TVec2& p1 , const TVec2& p2 , const CommonColor _color = White );
	void AddDebugRect ( const TVec2& p , const float w , const float h , const CommonColor _color = White );
	void AddDebugCircle ( const TVec2& p , const float r , const CommonColor _color = White );

	
	
	public:
		virtual void Process ( WorldState * worldState , GameSetting * setting , char * commands ) = 0;
		AI_Debug AIDebug;
};