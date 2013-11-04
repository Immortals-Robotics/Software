#include "ai09.h"

void ai09::WaitForPass ( int robot_num , bool chip )
{
	Line ball_line = Line::makeLineFromPositionAndAngle ( VecPosition ( ball.Position.X , ball.Position.Y ) , ball.velocity.direction );

	VecPosition ans = ball_line.getPointOnLineClosestTo ( VecPosition ( OwnRobot[robot_num].State.Position.X + BAR * cosDeg ( OwnRobot[robot_num].State.Angle ) , OwnRobot[robot_num].State.Position.Y + BAR * sinDeg ( OwnRobot[robot_num].State.Angle ) ) );

	//BAR = ans.getDistanceTo ( VecPosition ( ball.Position.X , ball.Position.Y ) ) / 2.0f;
	//if ( BAR < 86 )		BAR = 86;
	//if ( BAR > 600 )	BAR = 600;

	OwnRobot[robot_num].target.Angle = calculateOneTouchAngle ( robot_num , Vec2 (ans.getX() - BAR * cosDeg ( OwnRobot[robot_num].State.Angle ) , ans.getY() - BAR * sinDeg ( OwnRobot[robot_num].State.Angle ) ) );
	//OwnRobot[robot_num].face ( ball.Position );
	//OwnRobot[robot_num].face ( Vec2 ( -side*3025 , 0 ) );
	ERRTSetObstacles ( robot_num ,0,0,0,0,0);
	ERRTNavigate2Point ( robot_num , Vec2 (ans.getX() - BAR * cosDeg ( OwnRobot[robot_num].State.Angle ) , ans.getY() - BAR * sinDeg ( OwnRobot[robot_num].State.Angle ) ),0,100,&VELOCITY_PROFILE_MAMOOLI );
	//if ( DIS ( OwnRobot[robot_num].State.Position , ball.Position ) < 20 )
	if (chip) {
		OwnRobot[robot_num].Chip( 15 );
	}
	else
		OwnRobot[robot_num].Shoot( 15 );
	//OwnRobot[robot_num].Dribble( 15 );
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