#include "ai09.h"

float ai09::calculateOneTouchAngle ( int robot_num , TVec2 oneTouchPosition )
{
	//oneTouchPosition = ball.Position;
	double v0x , v0y;
    double v1x , v1y;
	
	v0x = ball.velocity.x;
	v0y = ball.velocity.y;
	
	
    double goalx = -side * field_width;
    double e;
	
	float goaly = 0;
	
	Line targetLine = Line::makeLineFromTwoPoints ( VecPosition ( -side * field_width , -100 ) , VecPosition ( -side * field_width , 100 ) );
	TVec2 boz = calculateOpenAngleToGoal (oneTouchPosition,robot_num);
	//boz.X = AngleWith(oneTouchPosition, Vec2(-side*field_width, 0));
	cout << "	Open angle : " << boz.X << "	" << boz.Y << endl;
	Line ball_line = Line::makeLineFromPositionAndAngle ( VecPosition ( oneTouchPosition.X , oneTouchPosition.Y ) , boz.X );
	
	VecPosition ans;
	Debug_Line * dbgLine;
	
	ans = Line::makeLineFromPositionAndAngle ( VecPosition ( oneTouchPosition.X , oneTouchPosition.Y ) , boz.X+boz.Y/2.0f ).getIntersection ( targetLine );
	/*dbgLine = AIDebug.add_line();
	dbgLine -> set_x1(ans.getX());
	dbgLine -> set_y1(ans.getY());
	dbgLine -> set_x2(oneTouchPosition.X);
	dbgLine -> set_y2(oneTouchPosition.Y);
	color * col = dbgLine -> mutable_col();
	col -> set_r(0);
	col -> set_g(255);
	col -> set_b(0);*/
	
	ans = Line::makeLineFromPositionAndAngle ( VecPosition ( oneTouchPosition.X , oneTouchPosition.Y ) , boz.X-boz.Y/2.0f ).getIntersection ( targetLine );
	/*dbgLine = AIDebug.add_line();
	dbgLine -> set_x1(ans.getX());
	dbgLine -> set_y1(ans.getY());
	dbgLine -> set_x2(oneTouchPosition.X);
	dbgLine -> set_y2(oneTouchPosition.Y);
	col = dbgLine -> mutable_col();
	col -> set_r(0);
	col -> set_g(255);
	col -> set_b(0);*/
	
	ans = ball_line.getIntersection ( targetLine );
	
	goalx = ans.getX ( );
	goaly = ans.getY ( );
	
	//cout << goalx << "	" << goaly << endl;
	
	/*dbgLine = AIDebug.add_line();
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
	dbgPoint -> set_r(50);*/
	
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