#include "ai09.h"
#include <math.h>

static int my_hys=0;
void ai09::GKHi ( int robot_num, bool stop )
{
	gkIntercepting = false;

	LOG_DEBUG("GKhi: {} _ {}", ballIsGoaling(), DIS(ball.Position, OwnRobot[robot_num].State.Position)/ball.velocity.magnitude);
	//side = -side;
	debugDraw = true;//TODO #10 Comment this in the game
	if(ballIsGoaling()){
		AddDebugCircle(OwnRobot[robot_num].State.Position,200,Red);
	}else{
		AddDebugCircle(OwnRobot[robot_num].State.Position,20,Yellow);
	}
	debugDraw = false;

	if ((ballIsGoaling())&&(DIS(ball.Position, OwnRobot[robot_num].State.Position)/ball.velocity.magnitude<5)&&(!stop))
	{
		//WaitForPass ( robot_num , true );
		GK_shirje(robot_num);
		my_hys = 10;
	}
	else if (( my_hys > 0 )&&(!stop))
	{
		//WaitForPass ( robot_num , true );
		GK_shirje(robot_num);
		my_hys --;
	}

	else
	{
		//OwnRobot[robot_num].Chip(50);
		
		my_hys = 0;
	
        side = - side;
		ERRTSetObstacles ( robot_num , false , false , false , false);
        side = -side;
		if ( ( IsInObstacle ( Vec2 ( (ball.Position.X),(ball.Position.Y) ) ) ) && ( ball.velocity.magnitude < 1500 ) && (!stop) && (side*ball.Position.X<field_width) && (fabs(ball.Position.Y)<penalty_area_width/2.0) )
		{
			LOG_DEBUG("GK intercepting");

			gkIntercepting = true;

			ERRTSetObstacles ( robot_num , 0 , 0 , 1 , 0);
			//tech_circle(robot_num,sgn(ball.Position.Y)*side*60 ,0,15,false);
			tech_circle(robot_num,AngleWith ( ball.Position , Vec2 ( side * (field_width+110) , 0 ) ) ,0,80,false,0,0,0);
		}
		else
		{
            float cornerStartMul = pow(max(0,1.2-timer.time()),1.1);
//			TVec2 target = GK_Ghuz_2018(cornerStartMul*0.4, cornerStartMul>0?(1+0.2*(1-cornerStartMul)):1,1);
//			TVec2 target = GK_Ghuz(cornerStartMul*0.4, cornerStartMul>0?(1+0.2*(1-cornerStartMul)):1,1);

			//Added by Dot_blue:
			float penalty_x = field_width - 85.0;
			float max_reach_y = (goal_width/2.0) + 20.0;
			float tmp_x,tmp_y;
			Line GK_Rail = Line::makeLineFromTwoPoints(VecPosition(side * penalty_x, 100),
													   VecPosition(side * penalty_x, -100));
			VecPosition ans;
			if(DIS(ball.Position,Vec2(side*field_width,0))<2500 && ball.Position.X > side*(field_width -1200) ){
				ans = GK_Rail.getPointOnLineClosestTo(VecPosition(ball.Position.X, ball.Position.Y));
				tmp_x = ans.getX();
				tmp_y = ans.getY();
			} else {
				tmp_x = side * penalty_x;
				tmp_y = (6.0 * ball.Position.Y) / 40.0f;
			}

			if (tmp_y < -max_reach_y) {
				tmp_y = -max_reach_y;
			}
			if (tmp_y > max_reach_y) {
				tmp_y = max_reach_y;
			}
			TVec2 target = Vec2(tmp_x, tmp_y);
			//Done by Dot_Blue TODO #9 test this...

			OwnRobot[robot_num].face(ball.Position);
			ERRTSetObstacles(robot_num, stop , false, false, false);
			ERRTNavigate2Point(robot_num, target, 0, 80, stop ? &VELOCITY_PROFILE_AROOM : &VELOCITY_PROFILE_MAMOOLI);

//			double R_robot = DIS(Vec2(-field_width, 0),OwnRobot[robot_num].State.Position);
//			double alpha_robot = AngleWith(Vec2(-field_width, 0),OwnRobot[robot_num].State.Position);
//			alpha_robot = min(90, max(-90, alpha_robot));
//			std::cout<<"GOALIE teta: "<<alpha_robot<<std::endl;
//			std::cout<<"GOALIE R: "<<R_robot<<std::endl;
//			std::cout<<"______________________"<<std::endl;
			
		}
	}
	//side = -side;
}

void ai09::GK_shirje ( int robot_num )
{
	LOG_DEBUG("GK shirje");

	Line ball_line = Line::makeLineFromPositionAndAngle ( VecPosition ( ball.Position.X , ball.Position.Y ) , ball.velocity.direction );
	VecPosition ans = ball_line.getPointOnLineClosestTo ( VecPosition ( OwnRobot[robot_num].State.Position.X , OwnRobot[robot_num].State.Position.Y ) );
	OwnRobot[robot_num].face(ball.Position);
	TVec2 fans = Vec2(ans.getX(), ans.getY());
	fans = ((fans-OwnRobot[robot_num].State.Position)*2.0f)+OwnRobot[robot_num].State.Position;
	ERRTSetObstacles ( robot_num ,0,0,0,0);
	ERRTNavigate2Point(robot_num,fans , 1, 100, &VELOCITY_PROFILE_KHARAKI);
	OwnRobot[robot_num].Chip(150);
}