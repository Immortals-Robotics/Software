/*
 * TITLE:     histogram.h
 *
 * PURPOSE:   This file wraps the histogram class for storing n-dimensional histograms
 *
 * WRITTEN BY: Brett Browning
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

#ifndef __HISTOGRAM_H__
#define __HISTOGRAM_H__

#include "rtypes.h"
#include "util.h"

#include <stdio.h>
#include "geometry.h"

#define DEF_MAX_SIZE 0x8000

class Histogram1D {
private:
  ushort *data;
  uint count;
  uint nr_bins;

  double maxv, minv;
  double hmean, hmeansq, hvariance;
  double scale;
  uint max_hist_size;

public:
  Histogram1D(double _maxv = 0.0, double _minv = 0.0,
	      uint _nr_bins = 0, uint _max_hist_size = DEF_MAX_SIZE) {
    data = NULL;
    set(_maxv, _minv, _nr_bins,_max_hist_size);
  }

  ~Histogram1D(void) {
    if (data != NULL)
      delete data;
  }

  bool add(double v);

  void set(double _maxv, double _minv, uint _nr_bins,
	   uint _max_hist_size = DEF_MAX_SIZE);

  inline ushort *getData(void) {
    return (data);
  }

  inline ushort e(double v);

  inline uint getSize(void) {
    return (nr_bins);
  }

  inline double mean(void) {
    return (hmean);
  }

  inline double variance(void) {
    return (hvariance);
  }
  inline double stdDev(void) {
    return (sqrt(hvariance));
  }

  void print(FILE *f);
  void print(void) {
    print(stdout);
  }
};

class Histogram2D {
private:
  ushort *data;
  uint count, datasize;
  uint nr_xbins, nr_ybins;

  vector2d maxv, minv;
  vector2d hmean, hmeansq, hvariance;
  vector2d scale;
  uint max_hist_size;

public:
  Histogram2D(void) {
    data = NULL;
    set(vector2d(0,0), vector2d(0,0));
  }
  Histogram2D(vector2d _maxv, vector2d _minv,
	      uint _nr_xbins = 0, uint _nr_ybins = 0, 
	      uint _max_hist_size = DEF_MAX_SIZE) {
    data = NULL;
    set(_maxv, _minv, _nr_xbins, _nr_ybins, _max_hist_size);
  }

  ~Histogram2D(void) {
    if (data != NULL)
      delete data;
  }

  bool add(vector2d v);

  void set(vector2d _maxv, vector2d _minv, uint _nr_xbins = 0, 
	   uint _nr_ybins = 0, uint _max_hist_size = DEF_MAX_SIZE);

  inline ushort *getData(void) {
    return (data);
  }

  inline ushort e(vector2d v);
  inline ushort e(uint xi, uint yi);

  inline uint getXSize(void) {
    return (nr_xbins);
  }
  inline uint getYSize(void) {
    return (nr_ybins);
  }

  inline vector2d mean(void) {
    return (hmean);
  }

  inline vector2d  variance(void) {
    return (hvariance);
  }
  inline vector2d stdDev(void) {
    return (vector2d(sqrt(hvariance.x), sqrt(hvariance.y)));
  }

  void print(FILE *f);
  void print(void) {
    print(stdout);
  }
};


#endif /* __HISTOGRAM_H__ */
