// GNU General Public License Agreement
// Copyright (C) 2004-2010 CodeCogs, Zyba Ltd, Broadwood, Holford, TA5 1DU, England.
//
// This program is free software; you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by CodeCogs. 
// You must retain a copy of this licence in all copies. 
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
// PARTICULAR PURPOSE. See the GNU General Public License for more details.
// ---------------------------------------------------------------------------------
//! Calculates the linear regression parameters and evaluates the regression line at arbitrary abscissas

#ifndef MATHS_REGRESSION_LINEAR_H
#define MATHS_REGRESSION_LINEAR_H

#include "Vector.h"
//! Given a set of points, this class calculates the linear regression parameters and evaluates the regression line at arbitrary abscissas.

class Linear
{
public:

	void calculate(int n, float *x, float *y);

	//! Evaluates the linear regression function at the given abscissa.
	float getValue(float x);

	//! Returns the slope of the regression line
	float getSlope();

	//! Returns the intercept on the Y axis of the regression line
	float getIntercept();

	//! Returns the linear regression coefficient
	float getCoefficient();
	
	bool isAmoodi();
	
	float getXIntercept();
	
	float getDisToPoint ( TVec2 p );
	
	void chapeKon ( void );

private:

	float m_a, m_b, m_coeff, xinter;
	bool amoodi;
};


//! A static function implementing the Linear Class for one off calculations

float Linear_once(int n, float *x, float *y, float a );

#endif

