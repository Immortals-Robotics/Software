#include "ai09.h"

int DBG_tracking_robot_ID = 7;

void ai09::ERRTSetObstacles ( int robot_num , bool bll , bool field , bool own , bool opp , bool dribble , bool bigPen )
{
	
	//robot_num = min(5,max(0,robot_num));
	clear_map ( );
//	if(OwnRobot[robot_num].State.vision_id == DBG_tracking_robot_ID)
//		debugDraw = true;
//	else
		debugDraw = true;

	const float base_robot_r = 90.0f;
	float robot_r = base_robot_r;

	if ( own )
	{
		for ( int i = 0 ; i < MAX_TEAM_ROBOTS ; i ++ )
		{
			if ( ( OwnRobot[i].State.seenState != CompletelyOut ) && ( i != robot_num ) && ( OwnRobot[i].State.vision_id != OwnRobot[robot_num].State.vision_id ) )
			{
				float other_r = base_robot_r;
				AddCircle ( OwnRobot[i].State.Position.X , OwnRobot[i].State.Position.Y , robot_r + other_r );
			}
		}
	}

	if ( opp )
	{
		for ( int i = 0 ; i < MAX_ROBOTS ; i ++ )
		{
			if ( OppRobot[i].seenState != CompletelyOut )
			{
				float other_r = base_robot_r;
				AddCircle ( OppRobot[i].Position.X , OppRobot[i].Position.Y , robot_r + other_r );
			}
		}
	}

	if ( bll )
	{
		AddCircle ( ball.Position.X , ball.Position.Y , 420.0f + robot_r );
	}

	if ( field )
	{
#ifndef NEW_FIELD_2018
        float penalty_circle_center_y = penalty_area_width/2.0;
		AddCircle ( side*field_width , -penalty_circle_center_y , penalty_area_r + 100.0f );
		AddCircle ( -side*field_width , -penalty_circle_center_y , penalty_area_r + 50.0f );

		AddCircle ( side*field_width , penalty_circle_center_y , penalty_area_r + 100.0f );
		AddCircle ( -side*field_width , penalty_circle_center_y , penalty_area_r + 50.0f );


		AddRectangle ( side*(field_width+85.0f) , -penalty_circle_center_y , -side*(185.0+penalty_area_r) , penalty_area_width );
        AddRectangle ( -side*(field_width+85.0f) , -penalty_circle_center_y , side*(135.0+penalty_area_r) , penalty_area_width );

		AddDebugCircle(Vec2(side*field_width , -penalty_circle_center_y),penalty_area_r + 100.0f,Cyan);
		AddDebugCircle(Vec2( -side*field_width , -penalty_circle_center_y),penalty_area_r + 50.0f,Cyan);

		AddDebugCircle(Vec2( side*field_width , penalty_circle_center_y),penalty_area_r + 100.0f,Cyan);
		AddDebugCircle(Vec2( -side*field_width , penalty_circle_center_y),penalty_area_r + 50.0f,Cyan);

		AddDebugRect( Vec2(side*(field_width+85.0f) , -penalty_circle_center_y) , -side*(185.0+penalty_area_r) , penalty_area_width,Cyan );
		AddDebugRect( Vec2(-side*(field_width+85.0f) , -penalty_circle_center_y), side*(135.0+penalty_area_r) , penalty_area_width,Cyan);
#else
        float penalty_area_half_width = penalty_area_width / 2.0f;
        AddRectangle ( -(field_width + 185.0f) , -(penalty_area_half_width + robot_r) , +(185.0f + penalty_area_r + robot_r) , penalty_area_width + 2.0f * robot_r );
		AddRectangle ( +(field_width + 185.0f) , -(penalty_area_half_width + robot_r) , -(185.0f + penalty_area_r + robot_r) , penalty_area_width + 2.0f * robot_r );
#endif
	}
	
	if ( bigPen )
	{
		float big_penalty_area_r  = penalty_area_r + 240.0f;
		float big_penalty_area_w  = penalty_area_width + 240.0f;
        float penalty_area_half_width = big_penalty_area_w / 2.0f;

		AddRectangle ( -side * (field_width + 185.0f) , -(penalty_area_half_width + robot_r) , side * (185.0f + big_penalty_area_r + robot_r) , big_penalty_area_w + 2.0f * robot_r );
	}
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