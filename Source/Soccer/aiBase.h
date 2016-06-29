#pragma once

#include <math/Vector.h>
#include <common_colors.h>
#include <protos/messages_immortals_configs.pb.h>
#include <protos/messages_immortals_aidebug.pb.h>
#include <protos/messages_immortals_world_state.pb.h>
#include <protos/messages_immortals_robot_wrapper.pb.h>

class aiBase
{
	protected:
	bool debugDraw;
	void AddDebugPoint ( const TVec2& p , const CommonColor _color = White );
	void AddDebugLine ( const TVec2& p1 , const TVec2& p2 , const CommonColor _color = White );
	void AddDebugRect ( const TVec2& p , const float w , const float h , const CommonColor _color = White );
	void AddDebugCircle ( const TVec2& p , const float r , const CommonColor _color = White );

	public:
		virtual void Process (
			const Immortals::Data::WorldState &worldState, 
			const Immortals::Data::GameConfig &setting,
			Immortals::Data::RobotMessageFrame *const message_frame) = 0;

		AI_Debug AIDebug;
};