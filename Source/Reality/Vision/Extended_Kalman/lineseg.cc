/*
 * TITLE:  LineSeg.cc
 *
 * PURPOSE:  This file encapsulates the line segment class used for
 *           determining when line segments intersect
 *
 * WRITTEN BY:  Scott Lenser, Brett Browning
 */
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

#include <math.h>
#include "lineseg.h"
#include "geometry.h"
#include "macros.h"

#include <stdio.h>

/**************************** TYPES ******************************************/


/**************************** CLASS IMPLEMENTATION ***************************/

/*
 * LineSeg -
 *
 * This function translates the line segment by the vector loc
 */
void LineSeg::Translate(vector2d loc) 
{
  start += loc;
  end += loc;
}

/*
 * Rotate -
 *
 * This function rotates the reference space for the line segment
 * by rotating the end point vectors
 */
void LineSeg::Rotate(double angle) 
{
  start = start.rotate(angle);
  end = end.rotate(angle);
}

/*
 * Gradient -
 *
 * Calculate the gradient for the line segment
 */
double LineSeg::Gradient(void)
{
  double	m;
  
  if (start.x != end.x)
    m = (end.y - start.y) / (end.x - start.x);
  else
    m = HUGE_VAL;
  return (m);
}

/*
 * Angle -
 *
 * Calculates the angle with respect to the positive x axis 
 * of the line segment
 */
double LineSeg::Angle(void)
{
  vector2d	p = end - start;
  return (p.angle());
}	

/*
 * Parallel -
 *
 * Returns a normalized vector parallel to the line 
 */
vector2d LineSeg::Parallel(void)
{
  vector2d	p = (end - start);
  p.normalize();
  return (p);
}	

/*
 * Parallel -
 *
 * Returns a vector parallel to the line from start to end
 */
vector2d LineSeg::LineVector(void)
{
  vector2d p = (end - start);
  return (p);
}

/*
 * Perp -
 *
 * Return a vector perpendicular to the line following
 * right hand coord system rules
 */
vector2d LineSeg::Perp(void)
{
  vector2d p = Parallel();
  return (p.perp());
}

/*
 * PtOnLine -
 *
 * Determins if the given point is on the line
 */
bool LineSeg::PtOnLine(vector2d p)
{
  double a1 = (end - start).angle();
  double a2 = (p - start).angle();
	
  return (a1 == a2);
}

/*
 * PtOnSeg -
 *
 * Check if the point is in the segment and on the line
 */
bool LineSeg::PtOnSeg(vector2d p)
{
  return (PtInSegRange(p) && PtOnLine(p));
}

/*
 * ReflectPoint -
 *
 * This function reflects the point p in the line segment
 */
vector2d LineSeg::ReflectPoint(vector2d p)
{
  vector2d	n = Perp();
	
  return (p + n * (2.0 * (start - p).dot(n)));
}

/*
 * PtInSeg -
 *
 * This function returns true if the point lies between the
 * segment end points. It doesn't check to see if the point
 * actually lies on the segmenet however
 *
 * rETURN VALUE: TRUe if point is between segment ends
 */
bool LineSeg::PtInSegRange(vector2d tp)
{
  vector2d hi(MAX(start.x, end.x), MAX(start.y, end.y));
  vector2d lo(MIN(start.x, end.x), MIN(start.y, end.y));
  
  return ((lo.x <= tp.x) && (tp.x <= hi.x) &&
	  (lo.y <= tp.y) && (tp.y <= hi.y));
}

/*
 * TurnDirection -
 *
 * This function returns the turning direction required from the line segment
 * to the point...is it a left turn or right turn
 *
 * rETURN VALUE: The turning direction
 */
int LineSeg::TurnDirection(vector2d p)
{
  vector2d p1 = end - start;
  p -= start;
  double c = p1.x * p.y - p1.y * p.x;
  
  if (c < 0)
    return (TURN_LEFT);
  else if (c == 0)
    return (TURN_STRAIGHT);
  else
    return (TURN_RIGHT);
}

/*
 * FindIntersectionPoint -
 *
 * finds the intersection point between this line segment and the given line segment
 * if it exists.  Return true if there is an intersection.  Sets result to be
 * a vector from the begining of the line segment to the intersection point
 *
 * RETURN VALUE: True if an intersection exists
 */
bool LineSeg::FindIntersectionPoint(LineSeg seg, vector2d &result) 
{
  double m1, m2;
  vector2d l2_start = seg.GetStart();
  
  m1 = Gradient();
  m2 = seg.Gradient();
  
  if (m1 == m2) {
    result = start;
    return (seg.PtOnLine(start));
  } else if (m1 == HUGE_VAL) {
    result.x = start.x;
    result.y = m2 * (start.x - l2_start.x) + l2_start.y;
  } else if (m2 == HUGE_VAL) {
    result.x = l2_start.x;
    result.y = m1 * (l2_start.x - start.x) + start.y;
  } else {
    double c1 = start.y - m1 * start.x;
    double c2 = l2_start.y - m2 * l2_start.x;
    result.x = (c2 - c1) / (m1 - m2);
    result.y = m1 * result.x + c1;
  }		
  
  /* check the end points */
  return (PtInSegRange(result) && seg.PtInSegRange(result));
}

/*
 * Reflect -
 *
 * This function reflects the line segment with respect to the other line segment
 */
void LineSeg::Reflect(LineSeg seg) 
{
  start = seg.ReflectPoint(start);
  end = seg.ReflectPoint(end);
}


/*
 * Distance -
 *
 * calculates the distance from the point to the closest 
 * point on the line segment. It sets closest_pt to this
 * closest point
 *
 * rETURN VALUE: The distance to the closest point
 */
double LineSeg::Distance(vector2d tp, vector2d &closest) 
{
  vector2d	n = Perp();
  vector2d	p = Parallel();
  double		d;
  
  d = (start - tp).dot(n);
  closest = tp + n * d;
  
  /* now check end points */
  double x = p.dot(start - tp);
  double xe = p.dot(end - start);
	
  if (x < 0)
    closest = start;
  else if (x > xe)
    closest = end;
  return (d);
}
