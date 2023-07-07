#include "ai09.h"

void ai09::ERRTSetObstacles(int robot_num, bool bll, bool field, bool own, bool opp)
{
	static constexpr float ownRobotRadius = 100.0f;
	static constexpr float oppRobotRadius = 200.0f;

	static constexpr float ballAreaRadius = 550.0f;

	// We allow errt points to be 250 mm outside the field,
	// so set this to some higher value
	static constexpr float penaltyAreaExtensionBehindGoal = 300.0f;
	static constexpr float bigPenaltyAddition = 250.0f;

	const bool ourPenalty = field || (robot_num != gk && !REF_playState->ourPlaceBall());
	const bool oppPenalty = field || !REF_playState->ourPlaceBall();

	const bool oppPenaltyBig = REF_playState->freeKick() || REF_playState->stop();

	clear_map();

	if (own || !REF_playState->gameOn())
	{
		for (int i = 0; i < Setting::kMaxOnFieldTeamRobots; i++)
		{
			if ((OwnRobot[i].State.seenState != CompletelyOut) && (i != robot_num) && (OwnRobot[i].State.vision_id != OwnRobot[robot_num].State.vision_id))
			{
				AddCircle(OwnRobot[i].State.Position.X, OwnRobot[i].State.Position.Y, 2 * ownRobotRadius);
				//AddDebugCircle(OwnRobot[i].State.Position,ownRobotRadius + (!dribble)*ownRobotRadius,Cyan);
			}
		}
	}

	if (opp || !REF_playState->gameOn())
	{
		for (int i = 0; i < Setting::kMaxRobots; i++)
		{
			if (OppRobot[i].seenState != CompletelyOut)
			{
				float opp_radius = oppRobotRadius;

				const float opp_dis_to_ball = DIS(OppRobot[i].Position, ball.Position);
				if (robot_num == attack && opp_dis_to_ball < oppRobotRadius)
				{
				    opp_radius = ownRobotRadius;
				}

				AddCircle(OppRobot[i].Position.X, OppRobot[i].Position.Y, opp_radius + ownRobotRadius);
				//AddDebugCircle(OppRobot[i].Position,ownRobotRadius + (!dribble)*ownRobotRadius,Cyan);
			}
		}
	}

	if (bll || !REF_playState->allowedNearBall())
	{
		AddCircle(ball.Position.X, ball.Position.Y, ballAreaRadius + ownRobotRadius);
	}

	const float penalty_area_half_width = penalty_area_width / 2.0f;

	if (ourPenalty)
	{
		AddRectangle(side * (field_width + penaltyAreaExtensionBehindGoal), -(penalty_area_half_width + ownRobotRadius), -side * (penaltyAreaExtensionBehindGoal + ownRobotRadius + penalty_area_r), penalty_area_width + 2 * ownRobotRadius);
	}

	if (oppPenalty)
	{
		AddRectangle(-side * (field_width + penaltyAreaExtensionBehindGoal), -(penalty_area_half_width + ownRobotRadius), side * (penaltyAreaExtensionBehindGoal + ownRobotRadius + penalty_area_r), penalty_area_width + 2 * ownRobotRadius);
	}

	if (oppPenaltyBig)
	{
		const float big_penalty_area_r = penalty_area_r + bigPenaltyAddition;
		const float big_penalty_area_w = penalty_area_width + bigPenaltyAddition;
		const float penalty_area_half_width = big_penalty_area_w / 2.0f;

		AddRectangle(-side * (field_width + penaltyAreaExtensionBehindGoal), -(penalty_area_half_width + ownRobotRadius), side * (penaltyAreaExtensionBehindGoal + ownRobotRadius + big_penalty_area_r), big_penalty_area_w + 2 * ownRobotRadius);
		//		AddDebugRect( Vec2(-side*(field_width+185.0f) , -penalty_circle_center_y - 200) , side*(385.0f+big_penalty_area_r) , penalty_area_width + 400,Purple );
	}

	// avoid the line between the ball and the placement point
	if (REF_playState->theirPlaceBall())
	{
		const TVec2 ball_line = *targetBallPlacement - ball.Position;
		const int ball_obs_count = std::ceil(Magnitude(ball_line) / (ballAreaRadius + ownRobotRadius));
		
		for (int i = 0; i < ball_obs_count; i++)
		{
		    const float t = (float)i / (float)ball_obs_count;
            const TVec2 ball_obs = ball.Position + ball_line * t;
            AddCircle(ball_obs.X, ball_obs.Y, ballAreaRadius + ownRobotRadius);
        }
	}
}
