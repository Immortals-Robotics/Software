#include "linear.h"

#include <math.h>

//! Given a set of points, this class calculates the linear regression parameters and evaluates the regression line at arbitrary abscissas.

void Linear::calculate(int n, float *x, float *y)
	{
		
		// calculate the averages of arrays x and y
		double xa = 0, ya = 0;
		for (int i = 0; i < n; i++) {
			xa += x[i];
			ya += y[i];
		}
		xa /= n;
		ya /= n;
		
		// calculate auxiliary sums
		double xx = 0, yy = 0, xy = 0;
		for (int i = 0; i < n; i++) {
			double tmpx = x[i] - xa, tmpy = y[i] - ya;
			xx += tmpx * tmpx;
			yy += tmpy * tmpy;
			xy += tmpx * tmpy;
		}
		
		// calculate regression line parameters
		amoodi = false;
		xinter = 0;
		m_a = 0;
		m_b = 0;
		m_coeff = 0;
		// make sure slope is not infinite
		if ( xx < 0.01 )
		{
			amoodi = true;
			xinter = xa;
			return;
		}
		
		m_b = xy / xx;
		if (fabs(m_b) > 50 ) {
			amoodi = true;
			xinter = xa;
			return;
		}
		m_a = ya - m_b * xa;
		m_coeff = (fabs(yy) == 0) ? 1 : xy / sqrt(xx * yy);
		
	}
	
	//! Evaluates the linear regression function at the given abscissa.
	
	float Linear::getValue(float x)
	{
		return m_a + m_b * x;
	}
	
	//! Returns the slope of the regression line
	
	float Linear::getSlope()
	{
		return m_b;
	}
	
	//! Returns the intercept on the Y axis of the regression line
	
	float Linear::getIntercept()
	{
		return m_a;
	}
	
	//! Returns the linear regression coefficient
	
	float Linear::getCoefficient()
	{
		return m_coeff;
	}

	bool Linear::isAmoodi()
	{
		return amoodi;
	}

	float Linear::getXIntercept()
	{
		return xinter;
	}

	float Linear::getDisToPoint ( TVec2 p )
	{
		if ( amoodi )
			return fabs(p.X-xinter);
		return fabs(m_b*p.X-p.Y+m_a)/sqrt(m_b*m_b+1.0);
	}

	void Linear::chapeKon ( void )
	{
		m_a = -m_a / m_b;
		m_b = 1.0f / m_b;
	}
	
//! A static function implementing the Linear Class for one off calculations

float Linear_once(int n, float *x, float *y, float a )
{
	// This function is created to enable an Instant Calculator on CodeCogs. 
	// You probably shouldn't be using this function otherwise. 
	
	Linear A;
	A.calculate(n, x, y);
	return A.getValue(a);
}