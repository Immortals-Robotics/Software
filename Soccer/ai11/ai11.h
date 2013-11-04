#pragma once

#include "aiBase.h"

#include "errt.h"
#include "tactic.h"

#include <iostream>
using namespace std;

#include "Geom.h"


class ai11 : public aiBase
{
	private:
	
	public:
		Tactic * OwnTactic[5];
		ai11 ( void );
		void Process ( WorldState * worldState , GameSetting * setting , char * commands );
};