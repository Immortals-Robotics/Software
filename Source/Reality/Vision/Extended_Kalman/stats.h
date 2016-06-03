/*
 * TITLE:        stats.h
 *
 * PURPOSE:      This file encapsulates teh stats class
 *               for holding and calculating frame stats information
 *               
 * WRITTEN BY:   Brett Browning, 
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

#ifndef __STATS_H__
#define __STATS_H__

#include "reality/net_vision.h"
#include <deque>
#include <stdio.h>

class FrameStats {
private:
  deque<net_vframe> vframes, sqframes, aframes;
  double window;
  bool verbose;
  
  net_vframe mean, sqmean, variances, acc, avgacc;
  net_vframe maxframe, maxacc;

  net_vframe missing;
  

  inline void AddVal(double &val, double add, double n);
  inline void SubVal(double &val, double sub, double n);

  inline void AddVal(float &val, float add, double _n);
  inline void SubVal(float &val, float sub, double _n);

  inline void AddVal(vector2f &val, vector2f add, double _n);
  inline void SubVal(vector2f &val, vector2f sub, double _n);

  inline void AddVal(vraw &val, vraw add, double n);
  inline void SubVal(vraw &val, vraw sub, double n);

  inline void SquareVal(vraw &val);
  void SquareVal(net_vframe &val);

  void AddVal(net_vframe  &val, net_vframe add, double n);
  void SubVal(net_vframe &val, net_vframe sub, double n);

  void CalculateVariance(net_vframe &res, net_vframe &sq, net_vframe &avg);
  void CalculateMaxValues(net_vframe &val, net_vframe &add);
  void CalculateAcceleration();

  void CountMissing(net_vframe &miss, net_vframe &latest, double n);
  void UnCountMissing(net_vframe &miss, net_vframe &latest, double n);

public:

  FrameStats(double _window = 2.0, bool _verbose = false);
  ~FrameStats(void) {}

  void SetVerbose(bool _verbose) {
    verbose = _verbose;
  }
  void SetWindow(double _window) {
    window = _window;
  }

  // update the frame structure with the latest information
  bool Update(net_vframe &latest);

  net_vframe *GetMissing(void){
    return (&missing);
  }
  
  net_vframe *GetMeanFrame(void) {
    return (&mean);
  }
  net_vframe *GetVarianceFrame(void) {
    return (&variances);
  }
  net_vframe *GetMaxFrame(void) {
    return (&maxframe);
  }

  net_vframe *GetAccFrame(void) {
   return (&avgacc);
  }

  net_vframe *GetMaxAcc(void) {
   return (&maxacc);
  }

  void Print(void) {
    Print(stderr);
  }

  void Print(FILE *f);
  
};


#endif // __STATS_H__


