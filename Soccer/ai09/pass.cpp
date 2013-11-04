#include "ai09.h"

void ai09::WaitForPass ( int robot_num , bool moving )
{
	moving = true;
	Line ball_line = Line::makeLineFromPositionAndAngle ( VecPosition ( ball.Position.X , ball.Position.Y ) , ball.velocity.direction );
	VecPosition ans = ball_line.getPointOnLineClosestTo ( VecPosition ( OwnRobot[robot_num].State.Position.X + BAR * cosDeg ( OwnRobot[robot_num].State.Angle ) , OwnRobot[robot_num].State.Position.Y + BAR * sinDeg ( OwnRobot[robot_num].State.Angle ) ) );

	float sBAR;
	if ( moving )
	{
		BAR = 83;
		ans = ball_line.getPointOnLineClosestTo ( VecPosition ( OwnRobot[robot_num].State.Position.X + BAR * cosDeg ( OwnRobot[robot_num].State.Angle ) , OwnRobot[robot_num].State.Position.Y + BAR * sinDeg ( OwnRobot[robot_num].State.Angle ) ) );
		sBAR = ans.getDistanceTo ( VecPosition ( ball.Position.X , ball.Position.Y ) );
		sBAR /= ball.velocity.magnitude;
		sBAR = ans.getDistanceTo ( VecPosition ( OwnRobot[robot_num].State.Position.X , OwnRobot[robot_num].State.Position.Y ) ) / sBAR;
		sBAR /= 39.0;
		//sBAR /= 1500000;
		if ( sBAR < 20 )		sBAR = 20;
		if ( sBAR > 100 )	sBAR = 100;
	}	
	//sBAR = min ( ans.getDistanceTo ( VecPosition ( OwnRobot[robot_num].State.Position.X , OwnRobot[robot_num].State.Position.Y ) ) , BAR );

	/*if ( ball.velocity.magnitude < 1500 )//DIS ( OwnRobot[robot_num].State.Position , ball.Position ) < 40 )
	{
		ERRTSetObstacles ( robot_num );
		//tech_circle(attack,AngleWith ( Vec2 ( -side * 3033 , 0 ) , ball.Position ) ,15);
		tech_circle(robot_num, NormalizeAngle( 180+calculateOpenAngleToGoal(ball.Position, robot_num).X), 15 );
		return;
			}
	else
		OwnRobot[robot_num].face ( ball.Position );*/
	OwnRobot[robot_num].target.Angle = calculateOneTouchAngle ( robot_num , Vec2 (ans.getX() - BAR * cosDeg ( OwnRobot[robot_num].State.Angle ) , ans.getY() - BAR * sinDeg ( OwnRobot[robot_num].State.Angle ) ) );

	
	ERRTSetObstacles ( robot_num );
	if ( moving )
	{
		BAR = -200;
		ERRTNavigate2Point ( robot_num , Vec2 (ans.getX() - BAR * cosDeg ( OwnRobot[robot_num].State.Angle ) , ans.getY() - BAR * sinDeg ( OwnRobot[robot_num].State.Angle ) ) , 0 , sBAR , &VELOCITY_PROFILE_KHARAKI);
	}
	else
		ERRTNavigate2Point ( robot_num , Vec2 (ans.getX() - BAR * cosDeg ( OwnRobot[robot_num].State.Angle ) , ans.getY() - BAR * sinDeg ( OwnRobot[robot_num].State.Angle ) ) );

	//if ( fabs ( NormalizeAngle ( OwnRobot[robot_num].State.Angle - AngleWith ( OwnRobot[robot_num].State.Position , Vec2 ( -side*3025 , 0 ) ) ) ) < 20 ) 
		OwnRobot[robot_num].Shoot( 15 );
	//else
		//OwnRobot[robot_num].Shoot( 0 );

	OwnRobot[robot_num].Dribble( 15 );
}

float ai09::calculateOneTouchAngle ( int robot_num , TVec2 oneTouchPosition )
{
	//oneTouchPosition = ball.Position;
	double v0x , v0y;
    double v1x , v1y;

	v0x = ball.velocity.x;
	v0y = ball.velocity.y;


    double goalx = -side * 3025;
    double e;

	float goaly = 0;

	Line targetLine = Line::makeLineFromTwoPoints ( VecPosition ( -side * 3025 , -100 ) , VecPosition ( -side * 3025 , 100 ) );
	TVec2 boz = calculateOpenAngleToGoal (oneTouchPosition,robot_num);
	//boz.X = AngleWith(ball.Position, Vec2(-side*3025, 0));
	cout << "	Open angle : " << boz.X << "	" << boz.Y << endl;
	Line ball_line = Line::makeLineFromPositionAndAngle ( VecPosition ( oneTouchPosition.X , oneTouchPosition.Y ) , boz.X );
	
	VecPosition ans;
	Debug_Line * dbgLine;
	
	ans = Line::makeLineFromPositionAndAngle ( VecPosition ( oneTouchPosition.X , oneTouchPosition.Y ) , boz.X+boz.Y/2.0f ).getIntersection ( targetLine );
	dbgLine = AIDebug.add_line();
	dbgLine -> set_x1(ans.getX());
	dbgLine -> set_y1(ans.getY());
	dbgLine -> set_x2(oneTouchPosition.X);
	dbgLine -> set_y2(oneTouchPosition.Y);
	color * col = dbgLine -> mutable_col();
	col -> set_r(0);
	col -> set_g(255);
	col -> set_b(0);
	
	ans = Line::makeLineFromPositionAndAngle ( VecPosition ( oneTouchPosition.X , oneTouchPosition.Y ) , boz.X-boz.Y/2.0f ).getIntersection ( targetLine );
	dbgLine = AIDebug.add_line();
	dbgLine -> set_x1(ans.getX());
	dbgLine -> set_y1(ans.getY());
	dbgLine -> set_x2(oneTouchPosition.X);
	dbgLine -> set_y2(oneTouchPosition.Y);
	col = dbgLine -> mutable_col();
	col -> set_r(0);
	col -> set_g(255);
	col -> set_b(0);
	
	ans = ball_line.getIntersection ( targetLine );

	goalx = ans.getX ( );
	goaly = ans.getY ( );

	//cout << goalx << "	" << goaly << endl;
	
	dbgLine = AIDebug.add_line();
	dbgLine -> set_x1(goalx);
	dbgLine -> set_y1(goaly);
	dbgLine -> set_x2(oneTouchPosition.X);
	dbgLine -> set_y2(oneTouchPosition.Y);
	col = dbgLine -> mutable_col();
	col -> set_r(255);
	col -> set_g(0);
	col -> set_b(0);
	
	Debug_Circle * dbgPoint = AIDebug.add_circle();
	dbgPoint -> set_x(goalx);
	dbgPoint -> set_y(goaly);
	dbgPoint -> set_r(50);

    //balls[0].setX(3000);
    //balls[0].setY(0);
    float aa = -sgn ( goalx ) * 90 ;
    float max = 6430000;
    float gg = 0;
    
    while(aa<180 - 90 * sgn ( goalx ) )
    {
        double a = (aa/180.0)*3.14;
		v1x = beta*(-sin(a)*v0x + cos(a)*v0y)*(-sin(a)) + shootK*cos(a) + gamma*(v0x - 2*(v0x*cos(a) + v0y*sin(a))*cos(a));            
        v1y = beta*(-sin(a)*v0x + cos(a)*v0y)*(cos(a))  + shootK*sin(a) + gamma*(v0y - 2*(v0x*cos(a) + v0y*sin(a))*sin(a));
        e = v1x * (-OwnRobot[robot_num].State.Position.Y+goaly) + v1y*(OwnRobot[robot_num].State.Position.X-goalx);
        if ( fabs ( e ) < max )
		{
			max = fabs ( e );
			gg = aa;
		}
		aa ++;
    }

	return NormalizeAngle ( gg );
}