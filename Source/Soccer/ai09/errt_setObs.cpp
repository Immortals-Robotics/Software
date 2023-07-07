#include "ai09.h"

void ai09::ERRTSetObstacles ( int robot_num , bool bll , bool field , bool own , bool opp)
{
	static constexpr float ownRobotRadius = 100.0f;
	static constexpr float oppRobotRadius = 150.0f;

	static constexpr float ballAreaRadius = 550.0f;

	static constexpr float penaltyAreaExtensionBehindGoal = 2 * ownRobotRadius;
	static constexpr float bigPenaltyAddition = 250.0f;


	bll = !REF_playState->allowedNearBall() || bll;

	opp = opp || !REF_playState->gameOn();
	own = own || !REF_playState->gameOn();

#if 0
	const bool ourPenalty = field;
	const bool oppPenalty = field;
#else
	const bool ourPenalty = field || (robot_num != gk && !REF_playState->ourPlaceBall());
	const bool oppPenalty = field || !REF_playState->ourPlaceBall();
#endif

	const bool oppPenaltyBig = REF_playState->freeKick() || REF_playState->stop();

	clear_map ( );

	if ( own )
	{
		for ( int i = 0 ; i < Setting::kMaxOnFieldTeamRobots ; i ++ )
		{
			if ( ( OwnRobot[i].State.seenState != CompletelyOut ) && ( i != robot_num ) && ( OwnRobot[i].State.vision_id != OwnRobot[robot_num].State.vision_id ) )
			{
				AddCircle ( OwnRobot[i].State.Position.X , OwnRobot[i].State.Position.Y , 2 * ownRobotRadius);
				//AddDebugCircle(OwnRobot[i].State.Position,ownRobotRadius + (!dribble)*ownRobotRadius,Cyan);
			}
		}
	}

	if ( opp )
	{
		for ( int i = 0 ; i < Setting::kMaxRobots ; i ++ )
		{
			if ( OppRobot[i].seenState != CompletelyOut )
			{
				AddCircle(OppRobot[i].Position.X, OppRobot[i].Position.Y, oppRobotRadius + ownRobotRadius);
				//AddDebugCircle(OppRobot[i].Position,ownRobotRadius + (!dribble)*ownRobotRadius,Cyan);
			}
		}
	}

	if (bll)
	{
		AddCircle ( ball.Position.X , ball.Position.Y , ballAreaRadius + ownRobotRadius);
	}

	const float penalty_area_half_width = penalty_area_width / 2.0f;

	if (ourPenalty)
	{
		AddRectangle(side * (field_width + penaltyAreaExtensionBehindGoal), -(penalty_area_half_width + ownRobotRadius), -side* (penaltyAreaExtensionBehindGoal + ownRobotRadius + penalty_area_r), penalty_area_width + 2 * ownRobotRadius);
	}

	if (oppPenalty)
	{
		AddRectangle(-side* (field_width + penaltyAreaExtensionBehindGoal), -(penalty_area_half_width + ownRobotRadius), side * (penaltyAreaExtensionBehindGoal + ownRobotRadius + penalty_area_r), penalty_area_width + 2 * ownRobotRadius);
	}

	if ( oppPenaltyBig )
	{
		const float big_penalty_area_r  = penalty_area_r + bigPenaltyAddition;
		const float big_penalty_area_w  = penalty_area_width + bigPenaltyAddition;
        const float penalty_area_half_width = big_penalty_area_w / 2.0f;

		AddRectangle ( -side * (field_width + penaltyAreaExtensionBehindGoal) , -(penalty_area_half_width + ownRobotRadius) , side * (penaltyAreaExtensionBehindGoal + ownRobotRadius + big_penalty_area_r) , big_penalty_area_w + 2 * ownRobotRadius);
//		AddDebugRect( Vec2(-side*(field_width+185.0f) , -penalty_circle_center_y - 200) , side*(385.0f+big_penalty_area_r) , penalty_area_width + 400,Purple );

	}
}
