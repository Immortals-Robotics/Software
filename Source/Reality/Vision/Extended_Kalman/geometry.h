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

#ifndef __GEOMETRY_H__
#define __GEOMETRY_H__

#include "vector.h"

//==== Vector types ====//

typedef Vector::vector2d<double> vector2d;
typedef Vector::vector3d<double> vector3d;
typedef vector2d point2d;
typedef vector3d point3d;
typedef Vector::vector2d_struct<double> vector2d_struct;

typedef Vector::vector2d<float> vector2f;
typedef Vector::vector3d<float> vector3f;
typedef vector2f point2f;
typedef vector3f point3f;

//==== Some conversion functions ====//


inline vector2f vstructtof(vector2d_struct &a)
{
  vector2f r((float) a.x, (float) a.y);
  return (r);
}

inline vector2d vstructtod(vector2d_struct &a)
{
  vector2d r(a.x, a.y);
  return (r);
}

inline void vftostruct(vector2f a, vector2d_struct &b)
{
  b.x = a.x;
  b.y = a.y;
}

inline void vdtostruct(vector2d a, vector2d_struct &b)
{
  b.x = a.x;
  b.y = a.y;
}

inline vector2f vdtof(vector2d &a)
{
  vector2f r(a.x,a.y);
  return(r);
}

inline vector2d vftod(vector2f &a)
{
  vector2d r(a.x,a.y);
  return(r);
}

inline vector3f vdtof(vector3d &a)
{
  vector3f r(a.x,a.y,a.z);
  return(r);
}

inline vector3d vftod(vector3f &a)
{
  vector3d r(a.x,a.y,a.z);
  return(r);
}

inline void vdtodarray(vector2d &a, double *array)
{
  array[0] = a.x;
  array[1] = a.y;
}

inline void vftodarray(vector2f &a, double *array)
{
  array[0] = (double) a.x;
  array[1] = (double) a.y;
}

inline void vdtofarray(vector2d &a, float *array)
{
  array[0] = (float) a.x;
  array[1] = (float) a.y;
}

inline void vftofarray(vector2f &a, float *array)
{
  array[0] = a.x;
  array[1] = a.y;
}

#endif // __VECTOR_H__
