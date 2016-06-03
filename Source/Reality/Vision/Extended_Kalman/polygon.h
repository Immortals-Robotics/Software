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

#ifndef __POLYGON_H__
#define __POLYGON_H__

#include "geometry.h"
#include "lineseg.h"

#include <stdio.h>

class Polygon {
public:
  Polygon();
  Polygon(int num_pts,const double *points_x, const double *points_y);
  Polygon(int num_pts, vector2d *points);
  Polygon(LineSeg seg,double dist,double angle);
  Polygon(const Polygon &p);
  ~Polygon() {FreePolygon();}

  void SetPolygon(int num_pts,const double *points_x, const double *points_y);
  void SetPolygon(int num_pts, vector2d *points);
  inline void FreePolygon();

  // rotate and translate the polygon -- rotate is applied first
  void RotateTranslate(double angle, vector2d pos);

  // rotate the polygon about the global origin
  void Rotate(double angle);

  inline int GetRawVertices(vector2d *points);
  int GetVertices(vector2d *points);
  inline vector2d *GetPointerToVertices(void);
  inline int GetRawSegments(LineSeg *sg);
  inline int GetSegments(LineSeg *sg);

  bool GetIntersectingLine(LineSeg seg, LineSeg &result, int *line_id);
  bool IsInside(vector2d p);
  bool IsInside(Polygon p);
  bool GetIntersectingLineSweep(vector2d p, double dist, double angle, 
				LineSeg &result, int *line_id);
  LineSeg ClosestSeg(vector2d p, vector2d &closest);
  void MoveOutside(vector2d &p);

  // printing/debug stuff
  void Print(FILE *f);
  void Print(void) {
    Print(stdout);
  }


public:
  int numPts; 
private:
  vector2d location;
  double orientation;

  vector2d bbox;

  vector2d *pts, *cornerpts;
  LineSeg *segs, *segs_orig;
};

#endif /* __POLYGON_H__ */

