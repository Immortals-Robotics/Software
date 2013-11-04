#include "ai09.h"
#include <math.h>

void ai09::DefGhuz ( int robot_num , TVec2 * defendTarget , bool stop )
{
	if ( !defendTarget )
		defendTarget = &(ball.Position);
	
	if (side==1) {
		ball.Position.X*=-1.0f;
		ball.Position.Y*=-1.0f;
	}

	double alpha = AngleWith(Vec2(-3025, 0),ball.Position);
	int alphaSgn = sgn(alpha);
	alpha = fabs(alpha);
	double tetta = (0.0001 * alpha * alpha * alpha)
	- (0.0066 * alpha * alpha )
	+ (1.0936 * alpha )
	- (4.9259 );
	
	double d = DIS(Vec2(-3025, 0),ball.Position);
	d=min(3000, d);
	
	/*double tetta = 550.2833
	+ 1.0787 * alpha
	- 0.2773 * d;*/
	tetta = min(54.6, tetta);
	tetta *= alphaSgn;
	
	d /= 1000.0;
	/*double R = ((- 0.0000000000002) * d * d * d * d * d)
	 + (0.000000002 * d * d * d * d)
	 - (0.00001 * d * d * d)
	 + (0.0214 * d * d)
	 - (22.378 * d)
	 + 9436.5;*/
	
	/*double R = 0.1285 * d
	+ 802.48;*/
	
	double R = -0.0671 * d * d * d * d
			+ 0.5089 * d * d * d
			- 1.2997 * d * d
			+ 1.3698 * d
			+ 0.4887;
	
	R *= 1000.0;
	
	
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
	
	ERRTSetObstacles ( robot_num , stop , true , true , false );
	OwnRobot[robot_num].face ( Vec2 ( (*defendTarget).X , (*defendTarget).Y ) );
	ERRTNavigate2Point(robot_num, target, 0, 100, &VELOCITY_PROFILE_MAMOOLI);
}