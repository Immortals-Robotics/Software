/**********************************************************************
 * 
 * Kwun Han <kwunh@cs.cmu.edu>
 * March 1997
 *
 * Michael Bowling <mhb@cs.cmu.edu>
 * 1998-2002
 *
 * Determinant and inverse code is copied from mtrxmath under the GPL.
 *
 **********************************************************************/
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

#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <memory.h>
#include <stdlib.h>
#include <math.h>

#include "matrix.h"

Matrix::Matrix(const char* const init_string)
{
  str_init(init_string);
}

Matrix::Matrix(int rows, int columns)
{
  r_ = rows;
  c_ = columns;

  mat = (double*)malloc(r_*c_*sizeof(double));
}

Matrix::Matrix(int rows, int columns, float *m)
{
  r_ = rows;
  c_ = columns;

  mat = (double*)malloc(r_*c_*sizeof(double));

  for(int i=0; i<rows*columns; i++)
    mat[i] = m[i];
}

Matrix::Matrix(int identity_size)
{
  r_ = identity_size;
  c_ = identity_size;

  mat = (double*)malloc(r_*c_*sizeof(double));
  for(int i = 0; i < identity_size; i++) {
    for(int j = 0; j < identity_size; j++) {
      this->e(i,j) = i==j;
    }
  }
}

Matrix::Matrix()
{
  //  printf("Matrix::Matrix()\n");

  r_ = 0;
  c_ = 0;
  mat = 0;    
}

Matrix::Matrix(const Matrix& other)
{
  //  fprintf(stderr, "Matrix::Matrix(const Matrix& other)\n");

  //  fprintf(stderr, "mat: 0x%x\n", mat);

  //  if(mat) free(mat);
  r_ = other.r_;
  c_ = other.c_;

  mat = (double*)malloc(r_*c_*sizeof(double));

  //  fprintf(stderr, "mat: 0x%x\n", mat);

  for(int i = 0; i < r_*c_; i++) {
    mat[i] = other.mat[i];
  }

  //  fprintf(stderr, "Matrix::Matrix(const Matrix& other) done\n");
}

Matrix::~Matrix()
{
  if(mat) free(mat);
}

void Matrix::CopyData(float *data)
{
  for (double *ptr = mat; ptr < mat + r_ * c_; ptr++)
    *data++ = (float) (*ptr++);
}

void Matrix::CopyData(double *data)
{
  memcpy(data, mat, r_ * c_ * sizeof(double));
}

const Matrix& Matrix::operator= (const Matrix& other)
{
  if(this == &other) return *this;

  //  fprintf(stderr, "\nop=\n");

  int i;

  //  fprintf(stderr, "mat: 0x%x\n", mat);

  if(mat) free(mat);
  r_ = other.r_;
  c_ = other.c_;

  mat = (double*)malloc(r_*c_*sizeof(double));

  //  other.print();

  //  fprintf(stderr, "mat: 0x%x r: %d c: %d\n",
  //	  mat, other.r_, other.c_);

  for(i = 0; i < (r_*c_); i++) {
    mat[i] = other.mat[i];
  }

  //  fprintf(stderr, "op= done\n");

  return *this;
}

const Matrix& Matrix::operator= (char* const init_string)
{
  if(mat) free(mat);
  str_init(init_string);

  return *this;
}

void Matrix::str_init(const char* const init_string)
{
  char* str1 = (char*)malloc(strlen(init_string)+1);
  int tcount;
  char* delim = " ;[]";


  strcpy(str1, init_string);

  tcount = 0;
  
  if(strtok(str1, delim)) tcount++;
  while(strtok(NULL, delim)) tcount ++;

  if(!tcount) {
    mat = 0;
    return;
  }

  mat = (double*)malloc(tcount * sizeof(double));

  strcpy(str1, init_string);

  r_ = 0;

  if(strtok(str1, ";")) r_++;
  while(strtok(NULL, ";")) r_ ++;

  c_ = tcount / r_;

  strcpy(str1, init_string);
  
  mat[0] = atof(strtok(str1, delim));
  for(int i = 1; i < tcount; i++) {
    mat[i] = atof(strtok(NULL, delim));
  }
  free(str1);
}

const Matrix operator+ (const Matrix& a, const Matrix& b)
{
  Matrix out;
  m_add(out, a, b);
  return out;
}

const Matrix operator- (const Matrix& a, const Matrix& b)
{
  Matrix out;
  m_subtract(out, a, b);
  return out;
}

const Matrix operator* (const Matrix& a, const Matrix& b)
{
  //  fprintf(stderr, "\nop*\n");

  //  a.print();
  //  b.print();

  Matrix out;
  m_multiply(out, a, b);

  //  out.print();

  //  fprintf(stderr, "op* done\n");

  return out;
}

const Matrix inverse(const Matrix& a)
{
  Matrix out;
  out = a;
  out.inverse();
  return out;
}

const Matrix transpose(const Matrix& a)
{
  Matrix out;
  out = a;
  out.transpose();
  return out;
}

const Matrix& m_multiply(Matrix& out, const Matrix& a, const Matrix& b)
{
  int i, j, k;

  assert(a.c_ == b.r_);

  out.resize(a.r_, b.c_);

//   fprintf(stderr, "%dx%d * %dx%d --> %dx%d\n",
// 	  a.r_, a.c_, b.r_, b.c_,
// 	  out.r_, out.c_);

  for(i = 0; i < out.r_*out.c_; i++)
    out.mat[i] = 0.0;

  for(i = 0; i < a.r_; i++) {
    for(j = 0; j < b.c_; j++) {
      for(k = 0; k < a.c_; k++) {
	out.e(i,j) += a.e(i,k) * b.e(k,j);
      }
    }
  }

  //  fprintf(stderr, "done\n");

  return out;
}

const Matrix& m_inverse(Matrix& out, const Matrix& in)
{
  out = in;
  out.inverse();
  return out;
}

const Matrix& m_add(Matrix& out, const Matrix& a, const Matrix& b)
{
  assert(a.r_ == b.r_ && a.c_ == b.c_);

  out.resize(a.r_, a.c_);

  for(int i = 0; i < a.r_ * a.c_ ; i++) {
    out.mat[i] = a.mat[i] + b.mat[i];
  }

  return out;
}

const Matrix& m_subtract(Matrix& out, const Matrix& a, const Matrix& b)
{
  assert(a.r_ == b.r_ && a.c_ == b.c_);

  out.resize(a.r_, a.c_);

  for(int i = 0; i < a.r_ * a.c_ ; i++) {
    out.mat[i] = a.mat[i] - b.mat[i];
  }

  return out;
}

const Matrix& m_transpose(Matrix& out, const Matrix& in)
{
  out.resize(in.c_, in.r_);

  for(int i = 0; i < in.r_; i++) {
    for(int j = 0; j < in.c_; j++) {
      out.e(j,i) = in.e(i,j);
    }
  }

  return out;
}

const Matrix& Matrix::transpose()
{
  if(!mat) return *this;

  double* newmat = (double*)malloc(r_*c_*sizeof(double));
  
  for(int i = 0; i < r_; i++) {
    for(int j = 0; j < c_; j++) {
      newmat[j*r_+i] = e(i,j);
    }
  }

  int t;
  t = c_;
  c_ = r_;
  r_ = t;    

  free(mat);
  mat = newmat;

  return *this;
}

const Matrix& Matrix::identity(int size)
{
  if(mat) free(mat);
  r_ = size;
  c_ = size;

  mat = (double*)malloc(r_*c_*sizeof(double));
  for(int i = 0; i < size; i++) {
    for(int j = 0; j < size; j++) {
      this->e(i,j) = i==j;
    }
  }
  return *this;
}

const Matrix& Matrix::resize(int rows, int columns)
{
  if (rows == r_ && columns == c_)
    return *this;

  if(mat) free(mat);

  r_ = rows;
  c_ = columns;

  mat = (double*)malloc(r_*c_*sizeof(double));

  return *this;
}

const Matrix& Matrix::scale(double factor)
{
  if(!mat) return *this;

  for(int i = 0; i < r_*c_; i++) {
    mat[i] *= factor;
  }
  return *this;
}

void Matrix::print() const
{
  int i,j;
  //printf("\n%dx%d\n", r_, c_);

  for(i = 0; i < r_; i++) {
    for(j = 0; j < c_; j++) {
      fprintf(stderr, "%f ", e(i,j));
    }
    fprintf(stderr, "\n");
  }
}

Matrix *Matrix::reduce_matrix (int cut_row, int cut_column) const
{
  Matrix *reduced;
  int y, x, rc, rr;
  rc=rr=0;
  
  reduced = new Matrix(r_-1, r_-1);

  /* This function performs a fairly simple function. It
     reduces a matrix in size by one element on each dimesion
     around the coordinates sent in the cut_row and cut_column
     values.  For example:
        3x3 Matrix:  1 2 3
	             4 5 6
		     7 8 9
	is sent to this function with the cut_row == 2 and 
	cut_column == 1, the function returns the 
	2x2 Matrix: 2 3
	            8 9
  */

  for ( x=0 ; x < r_ ; x++) {
    for ( y=0 ; y < c_; y++) {
      if( x == cut_row || y == cut_column ) { }
      else {
	reduced->e(rr,rc) = e(x,y);
	rc++;
      }
    }
    if (rr != cut_row || x > cut_row ) { rr++; rc=0; }
  }
  return reduced;
}

/* 
 * Determinant
 * Solve the determinant of a matrix
 *
 * Yes, I know this uses the Cramer's Method of finding a
 * determinate, and that Gaussian would be better.  I'm
 * looking into implementing a Gaussian function, but for
 * now this works. 
 */

double Matrix::determinant() const
{
  double det=0;
  Matrix *tmp;
  int i, sign=1;
  
  /* Do I need to explain this? */
  assert( r_ == c_ );

  /* This may never be used, but it's necessary for error
     checking */
  if (r_ == 1)
    return e(0,0);

  /* This may not be necessary, but it keeps the recursive function
     down to one less call, so it speeds things up just a bit */
  if (r_ == 2)
    return (e(0,0)*e(1,1)-e(0,1)*e(1,0));

  /* This is the recursive algorithm that does most of the computation. */
  else {
    for ( i=0 ; i < r_ ; i++, sign*=-1 ) {
      tmp = reduce_matrix(0,i);
      det += sign * e(0,i) * tmp->determinant();
      delete tmp;
    }
    return det;
  }
}

/*
 * inverse
 * 
 * Function to find the inverse of a matrix
 */

const Matrix &Matrix::inverse()
{
  Matrix *inverse;
  Matrix *tmp;
  double det=0;
  int row, col, sign=1;
  
  assert( r_ == c_ );

  inverse = new Matrix(r_,c_);

  det = determinant();
  assert( det != 0 );
  
  for ( row=0; row<r_; row++ ) {
    for ( col=0; col<c_; col++ ) {
      /* 
	 This looks kind of confusing.  All it does is take the 
	 inverse and multiply each square by the determinant that
	 is reduced around the spot the matrix is being reduced by
	 For Instance:

	 In a 3x3 Matrix: A B C
	                  D E F
			  G H I

	 When computing with Element B, Element B is multiplied by
	 the determinant of the 2x2 matrix: D F
	                                    G I
      */
      tmp = reduce_matrix(row,col);
      if ((row+col)%2 == 0) sign = 1;
      else sign = -1;
      inverse->e(col,row) = sign * tmp->determinant();
      delete tmp;
    }
  }
  
  inverse->scale(1/det);
  *this = *inverse;

  delete inverse;

  return *this;
}
