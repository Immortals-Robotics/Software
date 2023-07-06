#include "ai09.h"
#include <math.h>

double function_smooth(double low , double high , double x )
{
    if(x<=low)  return 0.0;
    else if (x>=high)   return 1.0;
    else    return ((x-low)/(high-low));
}

TVec2 ai09::GK_Ghuz_2018( float predictBallT, float rMul, int def_count ){

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
    alpha = min(90, max(-90, alpha));
    int alphaSgn = sgn(alpha);
    alpha = fabs(alpha);

    double d = DIS(Vec2(-field_width, 0),predictedBall);
    d = max(1000,min(d, 5000.0));


    double p0;
    double p1;
    double p2;
    double p3;
    double p4;
    double p5;

    double x = alpha;

    double R;
    double tetta;

    double tempVar;

    if (d > 1700)
    {




        if(0<=x && x<=33) {
            p4 = -0.0004;
            p3 = 0.0616;
            p2 = -2.7125;
            p1 = 53.025;
            p0 = 0.3598;
            R =p4*x*x*x*x+p3*x*x*x+p2*x*x+p1*x+p0;

        }else if(33<x && x<=43){
            tempVar=function_smooth(33,43,x);

            p4 = -0.0004;
            p3 = 0.0616;
            p2 = -2.7125;
            p1 = 53.025;
            p0 = 0.3598;
            R = (p4*x*x*x*x+p3*x*x*x+p2*x*x+p1*x+p0)*tempVar;

            p3 = -0.0609;
            p2 = 9.5675;
            p1 = -472.39;
            p0 = 8096.9;
            R += (p3*x*x*x+p2*x*x+p1*x+p0)*(1-tempVar);

        }
        else if(43<x && x<=65) {

            p3 = -0.0609;
            p2 = 9.5675;
            p1 = -472.39;
            p0 = 8096.9;
            R = p3*x*x*x+p2*x*x+p1*x+p0;


        }else if (65<x && x<=75){
            tempVar=function_smooth(65,75,x);

            p3 = -0.0609;
            p2 = 9.5675;
            p1 = -472.39;
            p0 = 8096.9;
            R = (p3*x*x*x+p2*x*x+p1*x+p0)*tempVar;

            p3 = 0.1045;
            p2 = -23.455;
            p1 = 1715.8;
            p0 = -40025;
            R += (p3*x*x*x+p2*x*x+p1*x+p0)*(1-tempVar);


        }else if (75<x && x<=90){

            p3 = 0.1045;
            p2 = -23.455;
            p1 = 1715.8;
            p0 = -40025;
            R = p3*x*x*x+p2*x*x+p1*x+p0;

        }



//        p5 = -9E-08;
//        p4 = 1E-05;
//        p3 = -7E-05;
//        p2 = -0.0633;
//        p1 = 3.4757;
//        p0 = 0.2358;

        p2 = -0.0107;
        p1 = 1.8559;
        p0 = 0.0;

        tetta = p2*x*x + p1*x + p0;
    }
    else
    {
        double p1;
        double p2;
        double p3;
        double p4;
        double p5;
        double p6;

        p1 =  8.718e-007;
        p2 =  -0.0001577;
        p3 =    0.007772;
        p4 =    -0.07465;
        p5 =     -0.6789;
        p6 =       802.1;
        R = p1*pow(x,5) + p2*pow(x,4) + p3*pow(x,3) + p4*pow(x,2) + p5*x + p6;

        p1 = -6.538e-008;
        p2 =  1.164e-005;
        p3 =  -0.0006758;
        p4 =     0.01326;
        p5 =      0.9203;
        p6 =       3.011;
        tetta = p1*pow(x,5) + p2*pow(x,4) + p3*pow(x,3) + p4*pow(x,2) + p5*x + p6;
    }

    R *= rMul * 1.0;
    tetta = min(81.6, tetta);
    tetta *= alphaSgn;

    TVec2 target = Vec2(-field_width, 0.0f);

    target.X += cosDeg(tetta) * R;
    target.Y += sinDeg(tetta) * R;

//    std::cout << "	x: " << x << "	tetta: " << tetta << "	R: " << R << std::endl;
//    std::cout << "ROBOT:" << target.X <<

    if (side==1) {
        target.X*=-1.0f;
        target.Y*=-1.0f;
    }
//    std::cout<<"BALL alpha: "<<alpha<<std::endl;

    return target;

}


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
	alpha = min(90, max(-90, alpha));
	int alphaSgn = sgn(alpha);
	alpha = fabs(alpha);

	double d = DIS(Vec2(-field_width, 0),predictedBall);
	d = max(1000,min(d, 5000.0));



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

	double R;
	double tetta;

	if (d > 1700)
	{
		p00 = -2805;
		p10 = 4.642;
		p01 = 89.43;
		p20 = -0.002308;
		p11 = -0.09415;
		p02 = -0.6572;
		p30 = 5.861e-007;
		p21 = 2.985e-005;
		p12 = 0.0009805;
		p03 = -0.00758;
		p40 = -7.13e-011;
		p31 = -3.646e-009;
		p22 = -2.837e-007;
		p13 = -8.228e-007;
		p04 = 8.083e-005;
		p50 = 3.241e-015;
		p41 = 1.475e-013;
		p32 = 2.204e-011;
		p23 = 3.227e-010;
		p14 = -3.989e-009;
		p05 = -2.616e-007;

		R =
			p00 + p10 * x + p01 * y + p20 * x * x + p11 * x * y + p02 * y * y + p30 * x * x * x + p21 * x * x * y +
			p12 * x * y * y + p03 * y * y * y + p40 * x * x * x * x + p31 * x * x * x * y + p22 * x * x * y * y +
			p13 * x * y * y * y + p04 * pow(y, 4) + p50 * pow(x, 5) + p41 * pow(x, 4) * y +
			p32 * pow(x, 3) * y * y + p23 * x * x * pow(y, 3) + p14 * x * pow(y, 4) + p05 * pow(y, 5);


		p00 = 1.995;
		p10 = 0.01943;
		p01 = -1.122;
		p20 = -1.377e-005;
		p11 = 0.002535;
		p02 = 0.01175;
		p30 = 4.331e-009;
		p21 = -9.288e-007;
		p12 = -1.773e-005;
		p03 = -0.0001188;
		p40 = -6.583e-013;
		p31 = 1.301e-010;
		p22 = 7.017e-009;
		p13 = -7.606e-008;
		p04 = 4.957e-006;
		p50 = 3.909e-017;
		p41 = -6.031e-015;
		p32 = -6.547e-013;
		p23 = -9.559e-013;
		p14 = 3.523e-010;
		p05 = -3.486e-008;
		tetta = p00 + p10 * x + p01 * y + p20 * x * x + p11 * x * y + p02 * y * y + p30 * x * x * x + p21 * x * x * y +
				p12 * x * y * y + p03 * y * y * y + p40 * x * x * x * x + p31 * x * x * x * y + p22 * x * x * y * y +
				p13 * x * y * y * y + p04 * pow(y, 4) + p50 * pow(x, 5) + p41 * pow(x, 4) * y +
				p32 * pow(x, 3) * y * y + p23 * x * x * pow(y, 3) + p14 * x * pow(y, 4) + p05 * pow(y, 5);
	}
	else
	{
		double p1;
		double p2;
		double p3;
		double p4;
		double p5;
		double p6;

		p1 =  8.718e-007;
		p2 =  -0.0001577;
		p3 =    0.007772;
		p4 =    -0.07465;
		p5 =     -0.6789;
		p6 =       802.1;
		R = p1*pow(y,5) + p2*pow(y,4) + p3*pow(y,3) + p4*pow(y,2) + p5*y + p6;

		p1 = -6.538e-008;
		p2 =  1.164e-005;
		p3 =  -0.0006758;
		p4 =     0.01326;
		p5 =      0.9203;
		p6 =       3.011;
		tetta = p1*pow(y,5) + p2*pow(y,4) + p3*pow(y,3) + p4*pow(y,2) + p5*y + p6;
	}

	R *= rMul * 1.0;
	tetta = min(81.6, tetta);
	tetta *= alphaSgn;

	TVec2 target = Vec2(-field_width, 0.0f);

	target.X += cosDeg(tetta) * R;
	target.Y += sinDeg(tetta) * R;

	//std::cout << "	alpha: " << alpha << "	tetta: " << tetta << "		d: " << d << "	R: " << R << std::endl;

	if (side==1) {
		target.X*=-1.0f;
		target.Y*=-1.0f;
	}
	std::cout<<"BALL alpha: "<<alpha<< std::endl;

	return target;
}
