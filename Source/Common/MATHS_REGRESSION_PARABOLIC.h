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
//! Approximates an arbitrary function using parabolic least squares fitting.
 
#ifndef MATHS_REGRESSION_PARABOLIC_H
#define MATHS_REGRESSION_PARABOLIC_H
 
//! Approximates an arbitrary function using parabolic least squares fitting.
 
class Parabolic{
public:
 
//! Class constructor
 
Parabolic ( );

Parabolic ( int n, float *x, float *y );

void calculate(int n, float *x, float *y);

//! Returns an approximated value.
 
float getValue(float x);
 
//! Returns the free coefficient \e a of the fitting function.
float get_a();
 
//! Returns the second coefficient \e b of the fitting function.
float get_b();
 
//! Returns the dominant coefficient \e c of the fitting function.
float get_c();

private:
 
float m_a, m_b, m_c;
};
 
 
//! A static function implementing the Parabolic Class for one off calculations
 
float Parabolic_once(int n, float *x, float *y, float a ); 
#endif
