#include "MATHS_REGRESSION_PARABOLIC.h"

Parabolic::Parabolic ( )
{
}

Parabolic::Parabolic ( int n, float *x, float *y )
{
	calculate ( n , x , y );
}
void Parabolic::calculate(int n, float *x, float *y)
{
 
	double a0(1), a1(0), a2(0), a3(0), a4(0);
	double b0(0), b1(0), b2(0);
 
            for (int m = 0; m < n; m++) {
 
                double xx = x[m], yy = y[m];
 
                a1 += xx; xx *= x[m];
                a2 += xx; xx *= x[m];
                a3 += xx; xx *= x[m];
                a4 += xx; xx  = x[m];
 
                b0 += yy;
                b1 += yy * xx;
                b2 += yy * xx * xx;
 
            }
            a1 /= n; a2 /= n; a3 /= n; a4 /= n;
            b0 /= n; b1 /= n; b2 /= n;
 
            double d = a0 * (a2 * a4 - a3 * a3) - a1 * (a1 * a4 - a2 * a3) + a2 * (a1 * a3 - a2 * a2);
 
            m_a = b0 * (a2 * a4 - a3 * a3) + b1 * (a2 * a3 - a1 * a4) + b2 * (a1 * a3 - a2 * a2);
            m_b = b0 * (a2 * a3 - a1 * a4) + b1 * (a0 * a4 - a2 * a2) + b2 * (a1 * a2 - a0 * a3);
            m_c = b0 * (a1 * a3 - a2 * a2) + b1 * (a2 * a1 - a0 * a3) + b2 * (a0 * a2 - a1 * a1);
            m_a /= d; m_b /= d; m_c /= d;
 
        }
 
//! Returns an approximated value.
 
float Parabolic::getValue(float x)
{
 
            return (m_c * x + m_b) * x + m_a;
 
        }
 
//! Returns the free coefficient \e a of the fitting function.
float Parabolic::get_a()
{
 
            return m_a;
 
        }
 
//! Returns the second coefficient \e b of the fitting function.
float Parabolic::get_b()
{
 
            return m_b;
 
        }
 
//! Returns the dominant coefficient \e c of the fitting function.
float Parabolic::get_c()
{
 
            return m_c;
 
        }
 
//! A static function implementing the Parabolic Class for one off calculations
 
float Parabolic_once(int n, float *x, float *y, float a )
{
  // This function is created to enable an Instant Calculator on CodeCogs. 
  // You probably shouldn't be using this function otherwise. 
 
   Parabolic A(n, x, y);
   return A.getValue(a);
}