#include "ai09.h"
#include <math.h>

#define world2mapX(a) min(729,max(0,((a/10)+364)))
#define world2mapY(a) min(529,max(0,((a/10)+264)))

static int my_hys=0;
void ai09::GK_Ghuz ( int robot_num , int defence_num , bool stop )
{
	Line ball_line = Line::makeLineFromPositionAndAngle ( VecPosition ( ball.Position.X , ball.Position.Y ) , ball.velocity.direction );
	Line targetLine = Line::makeLineFromTwoPoints ( VecPosition ( side * 3025 , -100 ) , VecPosition ( side * 3025 , 100 ) );
	VecPosition ballInter = ball_line.getIntersection(targetLine);
	
	if ((fabs(ballInter.getY())<440)&&(DIS(ball.Position, OwnRobot[robot_num].State.Position)/ball.velocity.magnitude<1)&&(fabs(ball.velocity.direction)>90))
	{
		WaitForPass ( robot_num , true );
		my_hys = 10;
	}
	else if ( my_hys > 0 )
	{
		WaitForPass ( robot_num , true );
		my_hys --;
	}
	
	else
	{
		OwnRobot[robot_num].Chip(15);
		
		my_hys = 0;
		
		ERRTSetObstacles ( robot_num , false , true , false , false , false );
		if ( ( IsInObstacle ( Vec2 ( world2mapX(ball.Position.X),world2mapY(ball.Position.Y) ) ) ) && ( ball.velocity.magnitude < 1000 ) && (!stop) && (side*ball.Position.X<3025) && (fabs(ball.Position.Y)<1100.0f) )
		{
			ERRTSetObstacles ( robot_num , 0 , 0 , 1 , 0 , 0 );
			//tech_circle(robot_num,sgn(ball.Position.Y)*side*60 ,0,15,false);
			tech_circle(robot_num,AngleWith ( ball.Position , Vec2 ( side * 3133 , 0 ) ) ,0,9,false,0,0,0);
		}
		else
		{
			if (side==1) {
				ball.Position.X*=-1.0f;
				ball.Position.Y*=-1.0f;
			}
			
			double alpha = AngleWith(Vec2(-3025, 0),ball.Position);
			int alphaSgn = sgn(alpha);
			alpha = fabs(alpha);
			/*double tetta = (0.000003 * alpha * alpha * alpha * alpha)
						- (0.0004 * alpha * alpha * alpha)
						+ (0.0183 * alpha * alpha)
						+ (0.7524 * alpha)
						+ 11.751;
			tetta *= alphaSgn;*/
			
			double d = DIS(Vec2(-3025, 0),ball.Position);
			d = max(1000,min(d, 3000.0));
			/*double R = ((- 0.0000000000002) * d * d * d * d * d)
					+ (0.000000002 * d * d * d * d)
					- (0.00001 * d * d * d)
					+ (0.0214 * d * d)
					- (22.378 * d)
					+ 9436.5;*/
			
			/*double R = 0.1659 * d
					+ 361.86;*/
			
			/*double tetta = 11.0504
						+ 0.9491 * alpha
						+ 0.0004 * d;
			*/
			double tetta = 10.4701
						+ 0.9126 * alpha
						+ 0.0005 * d;
			
			tetta = min(71.6, tetta);
			
			tetta *= alphaSgn;
			
			d = d / 1000.0;
			double R = 0.2062 * d * d * d * d
					- 1.7214 * d * d * d
					+ 5.1557 * d * d
					- 6.3613 * d
					+ 3.2991;
			R *= 1000.0;
			
			
			/*double R = -22419262.7398
					+ 0.1203 * alpha
					+ 22364.8223 * d
					- 0.0111 * alpha * alpha
					- 5.5775 * d * d;*/
			
			/*double R = 7320.5213
					- 1.1908 * alpha
					- 3.3427 * d;*/
			
			
			TVec2 target = Vec2(-3025.0f, 0.0f);
			
			target.X += cosDeg(tetta) * R;
			target.Y += sinDeg(tetta) * R;
			
			cout << "	alpha: " << alpha << "	tetta: " << tetta << "		d: " << d << "	R: " << R << endl;
			
			if (side==1) {
				ball.Position.X*=-1.0f;
				ball.Position.Y*=-1.0f;
				
				target.X*=-1.0f;
				target.Y*=-1.0f;
			}
			
			OwnRobot[robot_num].face(ball.Position);
			ERRTSetObstacles(robot_num, stop , false, false, false, false);
			ERRTNavigate2Point(robot_num, target, 0, 100, &VELOCITY_PROFILE_MAMOOLI);
			
		}
	}
}