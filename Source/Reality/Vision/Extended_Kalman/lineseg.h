/*
 * TITLE:  LineSeg.h
 *
 * PURPOSE:  This file encapsulates the line segment class used for
 *           determining when line segments intersect
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

#ifndef __LINESEG_H__
#define __LINESEG_H__

#include "geometry.h"

/* turn directions */
#define TURN_LEFT		-1
#define TURN_STRAIGHT		0
#define TURN_RIGHT		1

class LineSeg {
public:
  LineSeg(vector2d s, vector2d e) {
    start = s;
    end = e;
  }
  
  LineSeg(void) {
    start.set(0.0, 0.0);
    end.set(0.0, 0.0);
  }
  
  inline vector2d GetStart(void) {
    return (start);
  }
  
  inline vector2d GetEnd(void) {
    return (end);
  }
  
  inline double Length(void) const {
    return ((end - start).length());
  }
  
  double Gradient (void);
  double Angle (void);
  vector2d Parallel (void);
  vector2d Perp (void);
  
  vector2d LineVector(void);
  bool PtInSegRange(vector2d tp);
  bool PtOnLine(vector2d p);
  bool PtOnSeg(vector2d p);
  
  vector2d ReflectPoint(vector2d p);
  
  int TurnDirection(vector2d p);
  
  void Translate(vector2d loc);
  void Rotate(double angle);
  void Reflect(LineSeg seg);
  bool FindIntersectionPoint(LineSeg seg, vector2d &result);
  double Distance(vector2d tp, vector2d &closest);
private:
  vector2d	start,end;
};

#endif /* __LINESEG_H__ */

