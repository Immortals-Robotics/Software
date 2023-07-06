#include "ai09.h"

int DBG_tracking_robot_ID = 7;

void ai09::ERRTSetObstacles ( int robot_num , bool bll , bool field , bool own , bool opp , bool dribble)
{
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

	const bool bigPen = REF_playState->freeKick() || REF_playState->stop();

	//robot_num = min(5,max(0,robot_num));
	clear_map ( );
//	if(OwnRobot[robot_num].State.vision_id == DBG_tracking_robot_ID)
//		debugDraw = true;
//	else
		debugDraw = true;
	if ( own )
	{
		for ( int i = 0 ; i < MAX_TEAM_ROBOTS ; i ++ )
		{
			if ( ( OwnRobot[i].State.seenState != CompletelyOut ) && ( i != robot_num ) && ( OwnRobot[i].State.vision_id != OwnRobot[robot_num].State.vision_id ) )
			{
				//obstacle[obs_num].circle.setCenter ( VecPosition ( OwnRobot[i].Position.X , OwnRobot[i].Position.Y ) );
				//obstacle[obs_num].circle.setRadius ( 400 );
				AddCircle ( OwnRobot[i].State.Position.X , OwnRobot[i].State.Position.Y , 90.0f + (!dribble)*90.0f );
				//AddDebugCircle(OwnRobot[i].State.Position,90.0f + (!dribble)*90.0f,Cyan);
			}
		}
	}

	if ( opp )
	{
		for ( int i = 0 ; i < MAX_ROBOTS ; i ++ )
		{
			if ( OppRobot[i].seenState != CompletelyOut )
			{
				AddCircle ( OppRobot[i].Position.X , OppRobot[i].Position.Y , 150.0f + (!dribble)*90.0f );
//				AddDebugCircle(OppRobot[i].Position,90.0f + (!dribble)*90.0f,Cyan);
			}
			//obstacle[obs_num].circle.setCenter ( VecPosition ( OppRobot[i].Position.X , OppRobot[i].Position.Y ) );
			//obstacle[obs_num].circle.setRadius ( 400 );

		}
	}

	if (bll)
	{
		AddCircle ( ball.Position.X , ball.Position.Y , 510.0f );
	}

	const float penalty_area_half_width = penalty_area_width / 2.0f;

	if (ourPenalty)
	{
		AddRectangle(side * (field_width + 185.0f), -(penalty_area_half_width + 100.f), -side* (285.0f + penalty_area_r), penalty_area_width + 200.f);
	}

	if (oppPenalty)
	{
		AddRectangle(-side* (field_width + 185.0f), -(penalty_area_half_width + 100.f), side * (285.0f + penalty_area_r), penalty_area_width + 200.f);
	}

	if ( bigPen )
	{
		float big_penalty_area_r  = penalty_area_r + 240.0f;
		float big_penalty_area_w  = penalty_area_width + 240.0f;
        float penalty_area_half_width = big_penalty_area_w / 2.0f;

		AddRectangle ( -side * (field_width + 185.0f) , -(penalty_area_half_width + 100.f) , side * (285.0f + big_penalty_area_r) , big_penalty_area_w + 200.f );

//		AddDebugRect( Vec2(-side*(field_width+185.0f) , -penalty_circle_center_y - 200) , side*(385.0f+big_penalty_area_r) , penalty_area_width + 400,Purple );

	}
	
	/*if (robot_num == 0) {
		for ( int i = 1000 ; i < 3025 ; i += 10 )
		{
			for ( int j = -2025 ; j < 2025 ; j += 10 )
			{
				if ( IsInObstacle(Vec2(i,j)) )
				{
					Debug_Point * newDbgPoint = AIDebug.add_point();
					newDbgPoint -> set_x(i);
					newDbgPoint -> set_y(j);
				}
			}
		}
	}*/

	//AddRectangle ( -3260 , -440 , 34 , 88 );
	//AddRectangle ( 2940 , -440 , 34 , 88 );

	//AddRectangle ( 0 , 0 , 38 , 730 );

	//AddCircle ( world2mapX ( ball.Position.X ) , world2mapY ( ball.Position.Y ) , 7 );
	debugDraw = false;
}

void ai09::AddOppObs ( int mask1, int mask2 )
{
    for ( int i = 0 ; i < MAX_ROBOTS ; i ++ )
    {
        if ( OppRobot[i].seenState == CompletelyOut || i == mask1 || i == mask2 )
            continue;
        AddCircle ( OppRobot[i].Position.X , OppRobot[i].Position.Y , 180.0f );
    }
}