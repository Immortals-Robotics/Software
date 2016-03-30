#pragma once
#include "../../Common/timer.h"

class Tactic
{
	protected:
		Timer timer;
	public:
		virtual void Begin ( void ) = 0;
		virtual void End ( void ) = 0;
		virtual void Run ( void ) = 0;
};