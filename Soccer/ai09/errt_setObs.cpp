#include "ai09.h"
#include "obstacle.h"

void ai09::ERRTSetObstacles ( int robot_num , bool bll , bool field , bool own , bool opp , bool dribble , bool bigPen )
{
	
	robot_num = min(5,max(0,robot_num));
	clear_map ( );
	if ( own )
	{
		for ( int i = 0 ; i < 6 ; i ++ )
		{
			if ( ( OwnRobot[i].State.seenState != CompletelyOut ) && ( i != robot_num ) && ( OwnRobot[i].State.vision_id != OwnRobot[robot_num].State.vision_id ) )
			{
				//obstacle[obs_num].circle.setCenter ( VecPosition ( OwnRobot[i].Position.X , OwnRobot[i].Position.Y ) );
				//obstacle[obs_num].circle.setRadius ( 400 );
				AddCircle ( OwnRobot[i].State.Position.X , OwnRobot[i].State.Position.Y , 9 + (!dribble)*8 );
			}
		}
	}

	if ( opp )
	{
		for ( int i = 0 ; i < MAX_ROBOTS ; i ++ )
		{
			if ( OppRobot[i].seenState != CompletelyOut )
			{
				AddCircle ( OppRobot[i].Position.X , OppRobot[i].Position.Y , 9 + (!dribble)*8 );
			}
			//obstacle[obs_num].circle.setCenter ( VecPosition ( OppRobot[i].Position.X , OppRobot[i].Position.Y ) );
			//obstacle[obs_num].circle.setRadius ( 400 );
		}
	}

	if ( bll )
	{
		AddCircle ( ball.Position.X , ball.Position.Y , 51 );
	}

	if ( field )
	{
		AddCircle ( side*3020 , -170 , 95 );
		
		/*Debug_Circle * dbgCircle = AIDebug.add_circle();
		dbgCircle -> set_x(side*3020);
		dbgCircle -> set_y(-170);
		dbgCircle -> set_r(600);
		color * col = dbgCircle -> mutable_col();
		col -> set_r(255);
		col -> set_g(0);
		col -> set_b(0);*/
		
		AddCircle ( side*3020 , 180 , 95 );

		AddRectangle ( side*3110 , -170 , 104 , 36 );
		
		/*Debug_Rect * newDebugRect =  AIDebug.add_rect();
		newDebugRect -> set_x(side*3110);
		newDebugRect -> set_y(-170);
		newDebugRect -> set_w(360);
		newDebugRect -> set_h(690);*/
	}
	
	if ( bigPen )
	{
		AddCircle ( -side*3020 , -170 , 110 );
		
		AddCircle ( -side*3020 , 180 , 110 );
		
		AddRectangle ( -side*3110 , -170 , 119 , 36 );
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
}