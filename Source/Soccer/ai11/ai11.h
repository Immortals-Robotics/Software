#pragma once

#include "../aiBase.h"

#include "../ai09/errt/errt.h"
#include "tactic.h"

#include <iostream>
using namespace std;

class ai11 : public aiBase
{
	private:
	
	public:
		Tactic * OwnTactic[5];
		ai11 ( void );
		void Process ( WorldState * worldState , GameSetting * setting , char * commands );
};