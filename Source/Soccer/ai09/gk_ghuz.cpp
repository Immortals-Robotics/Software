#include "ai09.h"
#include <math.h>

TVec2 ai09::GK_Ghuz ( float predictBallT, float rMul, int def_count )
{
    TVec2 predictedBall = ball.Position;
    if ( predictBallT > 0.01 )
    {
        predictedBall = predictBallForwardAI(predictBallT);
    }
    
	if (side==1) {
		predictedBall.X*=-1.0f;
		predictedBall.Y*=-1.0f;
	}
	
	double alpha = AngleWith(Vec2(-field_width, 0),predictedBall);
	int alphaSgn = sgn(alpha);
	alpha = fabs(alpha);
	/*double tetta = (0.000003 * alpha * alpha * alpha * alpha)
	 - (0.0004 * alpha * alpha * alpha)
	 + (0.0183 * alpha * alpha)
	 + (0.7524 * alpha)
	 + 11.751;
	 tetta *= alphaSgn;*/
	
	double d = DIS(Vec2(-field_width, 0),predictedBall);
	d = max(1000,min(d, 3000.0));
	//d *= 0.5;
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
    
    R *= 1.3;
    R *= rMul * 1.0;
	
	
	/*double R = -22419262.7398
	 + 0.1203 * alpha
	 + 22364.8223 * d
	 - 0.0111 * alpha * alpha
	 - 5.5775 * d * d;*/
	
	/*double R = 7320.5213
	 - 1.1908 * alpha
	 - 3.3427 * d;*/
	
	
	TVec2 target = Vec2(-field_width, 0.0f);
	
	target.X += cosDeg(tetta) * R;
	target.Y += sinDeg(tetta) * R;
	
	//cout << "	alpha: " << alpha << "	tetta: " << tetta << "		d: " << d << "	R: " << R << endl;
	
	if (side==1) {
		target.X*=-1.0f;
		target.Y*=-1.0f;
	}
	
	return target;
}