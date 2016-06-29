#include "ai09.h"
#include <math.h>

TVec2 ai09::DefGhuz ( TVec2 * defendTarget )
{
	if ( !defendTarget )
		defendTarget = &(ball.Position);
	
	if (side==1) {
		ball.Position.X*=-1.0f;
		ball.Position.Y*=-1.0f;
	}

	double alpha = AngleWith(Vec2(-field_width, 0),ball.Position);
	alpha = min(90, max(-90, alpha));
	int alphaSgn = sgn(alpha);
	alpha = fabs(alpha);

	
	double d = DIS(Vec2(-field_width, 0),ball.Position);
	d = max(2000,min(d, 5000.0));

	double p00;
	double p10;
	double p01;
	double p20;
	double p11;
	double p02;
	double p30;
	double p21;
	double p12;
	double p03;
	double p40;
	double p31;
	double p22;
	double p13;
	double p04;
	double p50;
	double p41;
	double p32;
	double p23;
	double p14;
	double p05;

	double x = d;
	double y = alpha;

	p00 =        3994;
	p10 =      -4.968;
	p01 =       19.61;
	p20 =    0.002912;
	p11 =    -0.01335;
	p02 =     -0.5913;
	p30 =  -6.41e-007;
	p21 = -3.253e-006;
	p12 =   0.0007856;
	p03 =    0.001724;
	p40 =  5.485e-011;
	p31 =  2.306e-009;
	p22 = -2.352e-007;
	p13 = -4.385e-006;
	p04 =  3.782e-005;
	p50 = -1.132e-015;
	p41 = -2.596e-013;
	p32 =  2.293e-011;
	p23 =  1.121e-010;
	p14 =  2.953e-008;
	p05 =   -4.9e-007;

	double R = p00 + p10*x + p01*y + p20*x*x + p11*x*y + p02*y*y + p30 * x * x * x + p21*x*x*y + p12*x*y*y + p03*y*y*y + p40*x*x*x*x + p31*x*x*x*y + p22*x*x*y*y + p13*x*y*y*y + p04*pow(y,4) + p50*pow(x, 5) + p41*pow(x, 4)*y + p32*pow(x, 3)*y*y + p23*x*x*pow(y, 3) + p14*x*pow(y, 4) + p05*pow(y,5);

	p00 =       84.86;
	p10 =      -0.141;
	p01 =      0.9662;
	p20 =   8.73e-005;
	p11 =   0.0001852;
	p02 =   -0.002278;
	p30 = -2.572e-008;
	p21 = -1.465e-007;
	p12 =   2.97e-006;
	p03 = -7.938e-005;
	p40 =  3.626e-012;
	p31 =  3.805e-011;
	p22 = -5.399e-010;
	p13 = -1.519e-008;
	p04 =  1.809e-006;
	p50 = -1.969e-016;
	p41 = -3.312e-015;
	p32 =  7.918e-014;
	p23 = -3.233e-012;
	p14 =  1.762e-010;
	p05 = -1.373e-008;

	double tetta = p00 + p10*x + p01*y + p20*x*x + p11*x*y + p02*y*y + p30 * x * x * x + p21*x*x*y + p12*x*y*y + p03*y*y*y + p40*x*x*x*x + p31*x*x*x*y + p22*x*x*y*y + p13*x*y*y*y + p04*pow(y,4) + p50*pow(x, 5) + p41*pow(x, 4)*y + p32*pow(x, 3)*y*y + p23*x*x*pow(y, 3) + p14*x*pow(y, 4) + p05*pow(y,5);
	//tetta = min(80.0, tetta);
	tetta *= alphaSgn;

	TVec2 target = Vec2(-field_width, 0.0f);
	
	target.X += cosDeg(tetta) * R;
	target.Y += sinDeg(tetta) * R;
	
	//cout << "	alpha: " << alpha << "	tetta: " << tetta << "		d: " << d << "	R: " << R << endl;
	
	if (side==1) {
		ball.Position.X*=-1.0f;
		ball.Position.Y*=-1.0f;
		
		target.X*=-1.0f;
		target.Y*=-1.0f;
	}
	
	return target;
}