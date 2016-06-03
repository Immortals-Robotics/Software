/*
 * TTILE:		Polygon.cc
 *
 * PURPOSE:		This file encapsulates the polygon class.
 *
 * WRITTEN BY:	Scott Lenser, Brett Browning
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

#include <stdlib.h>
#include <math.h>

#include "polygon.h"
#include "util.h"

//#define DEBUG

#ifdef DEBUG
#include <stdio.h>
#endif

/**************************** POLYGON CLASS **********************************/

/*
 * Polygon -
 *
 * Empty constructor just safely initialises things
 */
Polygon::Polygon()
{
#ifdef DEBUG
  printf("POLYGON: creating empty polygon\n");
#endif
  location.set(0.0, 0.0);
  orientation = 0.0;
  pts = cornerpts = NULL;
  segs = NULL;
  segs_orig = NULL;
  numPts = 0;
  bbox.set(0,0);
}

/*
 * Polygon -
 *
 * Constructor for initiailising the class with a series of x and y arrays
 */
Polygon::Polygon(int num_pts, const double *points_x, const double *points_y)
{
  Polygon();
  SetPolygon(num_pts, points_x, points_y);
}

/*
 * Polygon -
 *
 * Constructor for initiailising the class with a series of x and y arrays
 */
Polygon::Polygon(int num_pts, vector2d *points)
{
  Polygon();
  SetPolygon(num_pts, points);
}
  
/*
 * Polygon -
 *
 * Constructor for initiailising the class with a line segment and a distance
 * and sweep angle
 */
Polygon::Polygon(LineSeg seg, double dist, double angle)
{
#ifdef DEBUG
  printf("POLYGON: creating sweep polygon \n");
#endif
	
  //  vector2d s = seg.GetStart();
  //  vector2d e = seg.GetEnd();
  
  double seg_dist = seg.Length();
  orientation = seg.Gradient();
  location = seg.GetStart();
  numPts = 4;
  
  double rel_angle = angle - orientation;
  
  pts = new vector2d[numPts];
  pts[0] = vector2d(0.0,0.0);
  pts[1] = vector2d(sin(rel_angle), cos(rel_angle)) * dist;
  
  pts[2] = vector2d(seg_dist, 0.0) + pts[1];
  pts[3] = vector2d(seg_dist, 0.0);

  cornerpts = new vector2d[numPts];
  for (int i = 0; i < numPts; i++) 
    cornerpts[i] = pts[i];
  
  segs = new LineSeg[numPts];
  for(int i = 0; i < numPts; i++)
    segs[i]=LineSeg(pts[i], pts[(i + 1) % numPts]);    
  segs_orig = new LineSeg[numPts];
  for(int i = 0; i < numPts; i++)
    segs_orig[i] = segs[i];
}

/*
 * Copy constructor -
 *
 */
Polygon::Polygon(const Polygon &p)
{
  SetPolygon(p.numPts,pts);
}

/*
 * SetPolygon -
 *
 * After creation setting of points -- must deallocate any allocated things
 */
void Polygon::SetPolygon(int num_pts,const double *points_x, const double *points_y)
{
#if 1

  location.set(0.0, 0.0);
  orientation = 0.0;
  numPts = num_pts;
  bbox.set(0,0);
  
  pts = new vector2d[numPts];
  cornerpts = new vector2d[numPts];
  for (int i = 0; i < numPts; i++) {
    pts[i].set(points_x[i], points_y[i]);
    cornerpts[i] = pts[i];
    bbox.x = max(bbox.x, points_x[i]);
    bbox.y = max(bbox.y, points_y[i]);
  }
  
  segs = new LineSeg[numPts];
  for(int i = 0; i < numPts; i++) {
    segs[i] = LineSeg(pts[i], pts[(i+1) % numPts]);
  }
  segs_orig = new LineSeg[numPts];
  for(int i = 0; i < numPts; i++)
    segs_orig[i] = segs[i];

#else
  vector2d	*points;
	
  points = new vector2d[num_pts];
  for (int i = 0; i < num_pts; i++)
    points[i].set(points_x[i], points_y[i]);
  SetPolygon(num_pts, points);

  delete(points);
#endif
}


/*
 * SetPolygon -
 *
 * After creation setting of points -- must deallocate any allocated things
 */
void Polygon::SetPolygon(int num_pts, vector2d *points)
{
  location.set(0.0, 0.0);
  orientation = 0.0;
  numPts = num_pts;
  bbox.set(0,0);
  
  pts = new vector2d[numPts];
  cornerpts = new vector2d[numPts];
  for (int i = 0; i < numPts; i++) {
    pts[i] = points[i];
    cornerpts[i] = points[i];
    bbox.x = max(bbox.x, points[i].x);
    bbox.y = max(bbox.y, points[i].y);
  }
  
  segs = new LineSeg[numPts];
  for(int i = 0; i < numPts; i++)
    segs[i] = LineSeg(pts[i], pts[(i+1) % numPts]);
  segs_orig = new LineSeg[numPts];
  for(int i = 0; i < numPts; i++)
    segs_orig[i] = segs[i];

#ifdef DEBUG
  printf("POLYGON: creating polygon with %i points\n", numPts);
  for (int i = 0; i < numPts; i++) {
    vector2d s = segs[i].GetStart();
    vector2d e = segs[i].GetEnd();
    printf("POLYGON: s %i (%3.0f, %3.0f) -> (%3.0f, %3.0f)\n", i, s.x, s.y, e.x, e.y);
  }
#endif
}

/*
 * FreePolygon -
 *
 * This funciton free's the memory allocation used in the polygon
 */
inline void Polygon::FreePolygon(void)
{
  if (pts)
    delete pts;
  if (cornerpts)
    delete cornerpts;
  if (segs)
    delete segs;
  if (segs_orig)
    delete segs_orig;
  pts = NULL;
  cornerpts = NULL;
  segs = NULL;
  segs_orig = NULL;
}

/*
 * SetPoint -
 *
 * This function sets the location and origin for the polygon and rotates
 * and translates the appropriate line segments for usage later by
 * the intersecting routines
 *
 * !!! NOTE : Rotate occurs before translation in current framework
 */
void Polygon::RotateTranslate(double angle, vector2d pos)
{
  location = pos;
  orientation = angle;

  for(int i = 0; i < numPts; i++) {
    segs[i] = segs_orig[i];
    segs[i].Rotate(orientation);
    segs[i].Translate(location);
  }
}

void Polygon::Rotate(double angle)
{
  orientation = angle;

  for(int i = 0; i < numPts; i++) {
    segs[i] = segs_orig[i];
    segs[i].Rotate(orientation);
  }
}


/*
 * GetRawPolygon -
 *
 * Fills in teh array for the  raw un-translated polygon points
 */
inline int Polygon::GetRawVertices(vector2d *points)
{
  memcpy(points, pts, numPts * sizeof(vector2d));
  return (numPts);
}

int Polygon::GetVertices(vector2d *points)
{
  for (int i = 0; i < numPts; i++)
    points[i] = segs[i].GetStart();
  return (numPts);
}

inline vector2d *Polygon::GetPointerToVertices(void)
{
  for (int i = 0; i < numPts; i++)
    cornerpts[i] = segs[i].GetStart();
  return (cornerpts);
}

inline int Polygon::GetRawSegments(LineSeg *sg)
{
  memcpy(sg, segs_orig, numPts * sizeof(LineSeg));
  return (true);
}

inline int Polygon::GetSegments(LineSeg *sg)
{
  memcpy(sg, segs, numPts * sizeof(LineSeg));
  return (true);
}



/*
 * GetIntersectingLine -
 *
 * find the first intersecting line segment of the line segment given
 * with the polygon. 
 * 
 * RETURN VALUE: returns false if no such line segment exists
 */
bool Polygon::GetIntersectingLine(LineSeg seg, LineSeg &result, int *line_id) 
{
  bool found = false;
  double min_int_dist = HUGE_VAL;
  bool intersects;
  vector2d int_pt(0.0, 0.0);
  
#ifdef DEBUG
  vector2d s = seg.GetStart();
  vector2d e = seg.GetEnd();
  printf("Test intersection : seg (%3.0f, %3.0f) -> (%3.0f, %3.0f)\n", s.x, s.y, e.x, e.y);
  for(int i = 0; i < numPts; i++) {
    vector2d s = segs[i].GetStart();
    vector2d e = segs[i].GetEnd();
    printf("\tsegment %i = ((%3.0f, %3.0f) -> (%3.0f, %3.0f)\n", i, s.x, s.y, e.x, e.y);
  }
  
#endif
	
  /* search over the segments (must be pre-translated/rotated)
   * looking for smallest intersection distance from the start of
   * the input segment
   */
  for(int i = 0; i < numPts; i++) {
    intersects = segs[i].FindIntersectionPoint(seg, int_pt);
    int_pt -= seg.GetStart();
    if (intersects && (int_pt.length() < min_int_dist)) {
      result = segs[i];
      *line_id = i;
      found = true;
    }		
  }
  
#ifdef DEBUG
  if (found) {
    vector2d s = result.GetStart();
    vector2d e = result.GetEnd();
    printf("POLYGON: found intersection @ line %i (%3.0f, %3.0f)\n", *line_id, int_pt.x, int_pt.y);
    printf("POLYGON: segment , seg (%3.0f, %3.0f) -> (%3.0f, %3.0f)\n", s.x, s.y, e.x, e.y);
  }
#endif

  return (found);
}

/*
 * IsInside -
 *
 * Determines if the point is inside the polygon or not based on the turning angle (+/-)
 * to the next point and the given point.
 */
bool Polygon::IsInside(vector2d p) 
{
  int turn_dir = 0;

  for (int i = 0; i < numPts; i++) {
    int dir = segs[i].TurnDirection(p);
    
#ifdef DEBUG
    vector2d s = segs[i].GetStart();
    vector2d e = segs[i].GetEnd();
    printf("POLYGON: seg %i p (%3.0f, %3.0f)\n", i, p.x, p.y);
    printf("POLYGON:  turn dir = %i, dir = %i\n", turn_dir, dir);
#endif
		
    if (((dir < 0) && (turn_dir > 0)) || ((dir > 0) && (turn_dir < 0)))
      return (false);
    turn_dir = dir;
  }
  return (true);
}


/*
 * GetIntersectingLineSweep -
 *
 * This function looks at the point given and sees if it is inside the polygon boundary
 * generated through a movement of <dist> and <angle> and determines if
 * it is inside the polygon.
 *
 * RETURN VALUE: TRUE if inside
 */
bool Polygon::GetIntersectingLineSweep(vector2d p, double dist, double angle, LineSeg &result, int *line_id) 
{
  for (int i = 0; i < numPts; i++) {
    Polygon test_poly(segs[i], dist, angle);
    
    if (test_poly.IsInside(p)) {
      result = segs[i];
      *line_id = i;
      return true;
    }
  }
  
  return false;
}

/*
 * ClosestSeg -
 *
 * This function returns the  segment that is closest to teh point
 * given
 *
 * RETURN VALUE: The segment that is closest and the point on that segment
 */
LineSeg Polygon::ClosestSeg(vector2d p, vector2d &closest) 
{
  double	min_dist = HUGE;
  LineSeg	best_seg;
  vector2d	closest_seg_pt;

  for(int i = 0; i < numPts; i++) {
    double dist;

    dist = segs[i].Distance(p, closest_seg_pt);
    if (dist < min_dist) {
      min_dist = dist;
      best_seg = segs[i];
      closest = closest_seg_pt;
    }
  }
  
  return (best_seg);
}

/*
 * MoveOutsiede -
 *
 * This funciton moves the point p outside of the polygon, by a fudge factor margin.
 */
void Polygon::MoveOutside(vector2d &p) 
{
  vector2d closest;
  
  if (!IsInside(p))
    return;
  
  /* find the closest segment first */		
  ClosestSeg(p, closest);
  
  /* shift p to the closest point plus a fudge factor */
  p += (closest - p) * 1.05;
}

void Polygon::Print(FILE *f)
{
  fprintf(f, "orig : ");
  for (int i = 0; i < numPts; i++) {
    vector2d s = segs_orig[i].GetStart();
    fprintf(f, "(%1.2f, %1.2f) ", s.x, s.y);
  }
  fprintf(f, "\nmoved : ");
  for (int i = 0; i < numPts; i++) {
    vector2d s = segs[i].GetStart();
    fprintf(f, "(%1.2f, %1.2f) ", s.x, s.y);
  }
  fprintf(f, "\n");
}

#if TEST_MAIN

#define NR_POINTS   4
double xpts[] = {0, 100, 100, 0};
double ypts[] = {0, 0, 100, 100};


int main(int argc, char *argv[])
{
  fprintf(stderr, "Testing polygon...\n");
  Polygon poly(NR_POINTS, xpts, ypts);

  vector2d p(50, 50);
  poly.SetPoint(p, M_PI_4);

  poly.Print();
  
}

#endif

