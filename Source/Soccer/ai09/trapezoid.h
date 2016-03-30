#pragma once

#include "../../Common/Vector.h"
#include "../../Reality/Vision/WorldState.h"

class TrapezoidPlanner
{
	private:
		TVec2 max_acc;
		TVec2 max_dec;
		TVec2 max_spd;

		float period;

		RobotState * init_state;
		RobotState * final_state;

		TVec3 oldAns;

		float Plan1D ( float d , float v , float tmp_max_spd , float tmp_max_acc , float tmp_max_dec );

	public:
		TrapezoidPlanner ( );
		TrapezoidPlanner ( TVec2 _max_acc , TVec2 _max_dec , TVec2 _max_spd , float _period );

		void init ( TVec2 _max_acc , TVec2 _max_dec , TVec2 _max_spd , float _period );

		TVec3 Plan (  RobotState * _init , RobotState * _final );
};