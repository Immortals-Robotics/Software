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

#ifndef __CRAPPY_MATRIX_H__
#define __CRAPPY_MATRIX_H__

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

class Matrix {

  // errr... rows and columns

  int r_;
  int c_;

  double* mat;

  void str_init(const char* const init_string);

  Matrix *reduce_matrix(int cut_row, int cut_col) const;
  double determinant() const;

public:
  Matrix(const char* const init_string);
  Matrix(int rows, int columns);
  Matrix(int rows, int columns, float *m);

  // this makes an identity matrix
  Matrix(int identity_size);

  // standard stuffs.
  Matrix();
  Matrix(const Matrix& other);

  ~Matrix();

  void CopyData(float *data);
  void CopyData(double *data);

  const Matrix& operator= (const Matrix& other);

  const Matrix& operator= (char* const init_string);

  friend const Matrix operator+ (const Matrix& a, const Matrix& b);
  friend const Matrix operator- (const Matrix& a, const Matrix& b);
  friend const Matrix operator* (const Matrix& a, const Matrix& b);
  friend const Matrix inverse(const Matrix& a);
  friend const Matrix transpose(const Matrix& a);

  friend const Matrix& m_multiply(Matrix& out, const Matrix& a,
				  const Matrix& b);
  friend const Matrix& m_inverse(Matrix& out, const Matrix& in);
  friend const Matrix& m_add(Matrix& out, const Matrix& a,
			     const Matrix& b);
  friend const Matrix& m_subtract(Matrix& out, const Matrix& a,
				  const Matrix& b);
  friend const Matrix& m_transpose(Matrix& out, const Matrix& in);

  const Matrix& transpose();
  const Matrix& identity(int size);
  const Matrix& inverse();
  const Matrix& resize(int row, int col);
  
  const Matrix& scale(double factor);

  inline double& e(int row, int col) const {return mat[row*c_+col];};

  int nrows() const { return r_; }
  int ncols() const { return c_; }

  void print() const;
};

#endif
