#pragma once

#include "WorldState.h"
#include "GameSetting.h"
#include "aidebug.pb.h"

class aiBase
{
	public:
		virtual void Process ( WorldState * worldState , GameSetting * setting , char * commands ) = 0;
		AI_Debug AIDebug;
};