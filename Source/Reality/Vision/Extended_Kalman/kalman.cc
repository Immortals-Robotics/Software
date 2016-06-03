// kalman.cc
//
// Generic extended kalman-bucy filter code.
//
// Created by:  Michael Bowling (mhb@cs.cmu.edu)
//
/* LICENSE:
  =========================================================================
    CMDragons'02 RoboCup F180 Source Code Release
  -------------------------------------------------------------------------
    Copyright (C) 2002 Manuela Veloso, Brett Browning, Mike Bowling,
                       James Bruce; {mmv, brettb, mhb, jbruce}@cs.cmu.edu
    School of Computer Science, Carnegie Mellon University
  -------------------------------------------------------------------------
    This software is distributed under the GNU General Public License,
    version 2.  If you do not have a copy of this licence, visit
    www.gnu.org, or write: Free Software Foundation, 59 Temple Place,
    Suite 330 Boston, MA 02111-1307 USA.  This program is distributed
    in the hope that it will be useful, but WITHOUT ANY WARRANTY,
    including MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  ------------------------------------------------------------------------- */

#define KALMAN_DEBUG 0

#include <stdio.h>
#include <math.h>

#include "kalman.h"

Kalman::Kalman(int _state_n, int _obs_n, double _stepsize) { 
  state_n = _state_n;
  obs_n = _obs_n;
  stepsize = _stepsize; 

  xs.clear(); xs.push_back(Matrix(state_n, 1));
  Ps.clear(); Ps.push_back(Matrix(state_n, state_n));
  Is.clear(); Is.push_back(Matrix());

  prediction_lookahead = 0.0;
  prediction_time = 0.0;
  errors = Matrix(_state_n, 1);
}

void Kalman::initial(double t, Matrix &x, Matrix &P)
{
  xs.clear(); xs.push_back(x);
  Ps.clear(); Ps.push_back(P);
  Is.clear(); Is.push_back(Matrix());
  stepped_time = time = t;
}

void Kalman::propagate()
{
  Matrix x = xs.back();
  Matrix P = Ps.back();
  Matrix &_A = A(x);
  Matrix &_W = W(x);
  Matrix &_Q = Q(x);
  Matrix I;
  
#if KALMAN_DEBUG
  fprintf(stderr, "PROPAGATE:\n");
  fprintf(stderr, "x =\n");
  x.print();
  fprintf(stderr, "P =\n");
  P.print();
#endif
  
  x = f(x, I);
  P = _A * P * transpose(_A) + _W * _Q * transpose(_W);

  xs.push_back(x);
  Ps.push_back(P);
  Is.push_back(I);
  
#if KALMAN_DEBUG
  fprintf(stderr, "=============>\nx =\n");
  x.print();
  fprintf(stderr, "P =\n");
  P.print();
  fprintf(stderr, "\n");
#endif

  stepped_time += stepsize;
}

void Kalman::update(const Matrix &z)
{
  Matrix x = xs.front();
  Matrix P = Ps.front();
  Matrix I = Is.front();
  Matrix &_H = H(x);
  Matrix &_V = V(x); 
  Matrix &_R = R(x);

  // We clear the prediction list because we have a new observation.
  xs.clear(); Ps.clear(); Is.clear(); stepped_time = time;

  Matrix K = P * transpose(_H) * 
    inverse(_H * P * transpose(_H) + _V * _R * transpose(_V));

  Matrix error = K * (z - h(x));

#if KALMAN_DEBUG
  fprintf(stderr, "UPDATE:\n");
  fprintf(stderr, "x =\n");
  x.print();
  fprintf(stderr, "z =\n");
  z.print();
  fprintf(stderr, "P =\n");
  P.print();
  fprintf(stderr, "K =\n");
  K.print();
#endif
  
  x = x + error;
  P = (Matrix(P.nrows()) - K * _H) * P;

  // Add the current state back onto the prediction list.
  xs.push_back(x); Ps.push_back(P); Is.push_back(I);

  if (prediction_lookahead > 0.0) {
    if (time - prediction_time >= prediction_lookahead) {

      if (prediction_time > 0.0) {
	Matrix error = x - prediction_x;

	for(int i=0; i < error.nrows(); i++)
	  errors.e(i, 0) += fabs(error.e(i, 0));
	errors_n++;
      }

      prediction_x = predict(prediction_lookahead);
      prediction_time = time;
    }
  }

#if KALMAN_DEBUG
  fprintf(stderr, "=============>\nx =\n");
  x.print();
  fprintf(stderr, "P =\n");
  P.print();
  fprintf(stderr, "\n");
#endif
}

void Kalman::tick(double dt) 
{
  uint nsteps = (int) rint(dt / stepsize);

  while(xs.size() - 1 < nsteps) propagate();

  xs.erase(xs.begin(), xs.begin() + nsteps);
  Ps.erase(Ps.begin(), Ps.begin() + nsteps);
  Is.erase(Is.begin(), Is.begin() + nsteps);
  
  time += dt;
}

Matrix Kalman::predict(double dt)
{
  uint nsteps = (int) rint(dt / stepsize);

  while(xs.size() - 1 < nsteps) propagate();

  return xs[nsteps];
}

Matrix Kalman::predict_cov(double dt)
{
  uint nsteps = (int) rint(dt / stepsize);

  while(xs.size() - 1 < nsteps) propagate();

  return Ps[nsteps];
}

Matrix Kalman::predict_info(double dt)
{
  uint nsteps = (int) rint(dt / stepsize);

  while(xs.size() - 1 < nsteps) propagate();

  return Is[nsteps];
}

Matrix Kalman::predict_fast(double dt)
{
  uint nsteps = (int) rint(dt / stepsize);
  double orig_stepsize = stepsize;

  if (xs.size() - 1 >= nsteps) return xs[nsteps];

  stepsize = dt - (stepped_time - time);
  propagate();

  Matrix rv = xs.back();

  stepped_time -= stepsize;
  stepsize = orig_stepsize;
  xs.pop_back();
  Ps.pop_back();
  Is.pop_back();

  return rv;
}

double Kalman::obs_likelihood(double dt, Matrix &z)
{
  Matrix x = predict(dt);
  Matrix P = predict_cov(dt);
  Matrix _hx = h(x);
  Matrix &_H = H(x);

  Matrix C = _H * P * transpose(_H);

  Matrix D = z - _hx;
  
  double likelihood = 1.0;

  for(int i=0; i<D.nrows(); i++)
    likelihood *= exp( - (D.e(i, 0) * D.e(i, 0)) / (2 * C.e(i, i)) );

  return likelihood;
}

Matrix Kalman::error_mean()
{
  return errors.scale(1.0 / (double) errors_n);
}

void Kalman::error_reset()
{
  errors = errors.scale(0.0);
  errors_n = 0;
}

double Kalman::error_time_elapsed()
{
  return errors_n * prediction_lookahead;
}

