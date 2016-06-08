/*========================================================================
    Util.h : Numerical utility functions
  ------------------------------------------------------------------------
    Copyright (C) 1999-2002  James R. Bruce
    School of Computer Science, Carnegie Mellon University
  ------------------------------------------------------------------------
    This software is distributed under the GNU General Public License,
    version 2.  If you do not have a copy of this licence, visit
    www.gnu.org, or write: Free Software Foundation, 59 Temple Place,
    Suite 330 Boston, MA 02111-1307 USA.  This program is distributed
    in the hope that it will be useful, but WITHOUT ANY WARRANTY,
    including MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  ========================================================================*/

#ifndef __UTIL_H__
#define __UTIL_H__

#include <math.h>
// #include <cstddef>
#include <algorithm>
using namespace std;

#ifndef M_2PI
#define M_2PI 6.28318530717958647693
#endif

#ifndef __SGI_STL_ALGOBASE_H
#ifndef __SGI_STL_INTERNAL_ALGOBASE_H

template <class num>
inline num max(num a,num b)
{
  return((a > b)? a : b);
}

template <class num>
inline num min(num a,num b)
{
  return((a < b)? a : b);
}

template <class data>
inline void swap(data &a,data &b)
{
  data t;
  t = a;
  a = b;
  b = t;
}

#endif
#endif

template <class num1,class num2>
inline num1 bound(num1 x,num2 low,num2 high)
{
  if(x < low ) x = low;
  if(x > high) x = high;
  return(x);
}

template <class num1,class num2>
inline num1 abs_bound(num1 x,num2 range)
// bound absolute value x in [-range,range]
{
  if(x < -range) x = -range;
  if(x >  range) x =  range;
  return(x);
}

template <class num>
inline num max3(num a,num b,num c)
{
  if(a > b){
    return((a > c)? a : c);
  }else{
    return((b > c)? b : c);
  }
}

template <class num>
inline num min3(num a,num b,num c)
{
  if(a < b){
    return((a < c)? a : c);
  }else{
    return((b < c)? b : c);
  }
}

template <class num>
inline num max_abs(num a,num b)
{
  return((fabs(a) > fabs(b))? a : b);
}

template <class num>
inline num min_abs(num a,num b)
{
  return((fabs(a) < fabs(b))? a : b);
}

template <class num>
inline void sort(num &a,num &b,num &c)
{
  if(a > b) swap(a,b);
  if(b > c) swap(b,c);
  if(a > b) swap(a,b);
}

template <class real>
real sq(real x)
{
  return(x * x);
}

template <class num>
num sign_nz(num x)
{
  return((x >= 0)? 1 : -1);
}

template <class num>
num sign(num x)
{
  return((x >= 0)? (x > 0) : -1);
}

template <class real>
real fmodt(real x,real m)
// Does a real modulus the *right* way, using
// truncation instead of round to zero.
{
  return(x - floor(x / m)*m);
}

// Returns angle within [-PI,PI]
template <class real>
real angle_mod(real a)
{
  a -= M_2PI * rint(a / M_2PI);

  return(a);
}

// Returns angle within [-PI,PI]
template <class real>
real anglemod(real a)
{
  a -= M_2PI * rint(a / M_2PI);

  return(a);
}

// Returns difference of two angles (a-b), within [-PI,PI]
template <class real>
real angle_diff(real a,real b)
{
  real d;

  d = a - b;
  d -= M_2PI * rint(d / M_2PI);

  return(d);
}

template <class data>
inline int mcopy(data *dest,data *src,int num)
{
  int i;

  for(i=0; i<num; i++) dest[i] = src[i];

  return(num);
}

template <class data>
inline data mset(data *dest,data val,int num)
{
  int i;

  for(i=0; i<num; i++) dest[i] = val;

  return(val);
}

template <class data>
inline void mzero(data &d)
{
  memset(&d,0,sizeof(d));
}

template <class data>
inline void mzero(data *d,int n)
{
  memset(d,0,sizeof(data)*n);
}

template <class node>
int list_length(node *list)
{
  node *p = list;
  int num = 0;

  while(p){
    num++;
    p = p->next;
  }

  return(num);
}

/*
template <class node>
node *random_element(node *list)
{
  node *p = list;
  int num = 0;

  while(p){
    num++;
    p = p->next;
  }

  return(num);
}
*/

#endif
