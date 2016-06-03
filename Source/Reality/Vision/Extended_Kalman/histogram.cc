/*
 * TITLE:     histogram.cc
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

#include "histogram.h"

void Histogram1D::set(double _maxv, double _minv, uint _nr_bins,
		      uint _max_hist_size = DEF_MAX_SIZE)
{
  nr_bins = _nr_bins;
  maxv = _maxv;
  minv = _minv;

  if (data != NULL)
    delete data;
  data = NULL;

  count = 0;
  max_hist_size = _max_hist_size;
  hmean = hmeansq = hvariance = 0.0;

  if (nr_bins == 0) 
    return;

  scale =  (double) nr_bins / (maxv - minv);
  data = new ushort[nr_bins];

  memset(data, 0, sizeof(ushort) * nr_bins);
}



bool Histogram1D::add(double v)
{
  // may need to use floor(x + 0.5) to center properely

  double vrem = minv;
  int indx = (int) ((v - minv) * scale);

  if ((indx < 0) || (indx >= (int) nr_bins))
    return false;

  // add to histogram
  data[indx]++;
  count++;

  // add the truncation - should mabye add 0.5 to center it
  v = (((double) indx + 0.5) / scale) + minv;

  // udpate means and variances here
  double n = (double) count;
  hmean += (v - hmean) / n;
  hmeansq += (v * v - hmeansq) / n;
  hvariance = hmeansq - hmean * hmean;

  // now remove a value from the histogram if we have enough
  if (count > max_hist_size) {
    do {
      indx = rand() % nr_bins;
    } while (data[indx] == 0);
    data[indx]--;
    count--;
    vrem = (((double) indx + 0.5) / scale) + minv;

    n = (double) count;
    hmean += (hmean - vrem) / n;
    hmeansq += (hmeansq - vrem * vrem) / n;
    hvariance = hmeansq - hmean * hmean;
  }

  return true;
}

inline ushort Histogram1D::e(double v) 
{
  int indx = (int) ((v - minv) * scale);
  if ((indx < 0) || (indx >= (int) nr_bins))
    return 0;
  return (data[indx]);
}

void Histogram1D::print(FILE *f)
{
  fprintf(f, "Histogram 1D: maxv %f minv %f nr_bins %i maxcount %i, count %i\n",
	  maxv, minv, nr_bins, max_hist_size, count);
  fprintf(f, "\t mean %f variance %f sdev %f\n",
	  hmean, hvariance, sqrt(hvariance));

  for (uint i = 0; i < nr_bins; i++) {
    fprintf(f, "% 4i, ", data[i]);
    if ((i + 1) % 11 == 0)
      fprintf(f, "\n");
  }
}

void Histogram2D::set(vector2d _maxv, vector2d _minv, uint _nr_xbins, 
		      uint _nr_ybins, uint _max_hist_size = DEF_MAX_SIZE)
{
  nr_xbins = _nr_xbins;
  nr_ybins = _nr_ybins;
  maxv = _maxv;
  minv = _minv;

  if (data != NULL)
    delete data;
  data = NULL;

  count = 0;
  max_hist_size = _max_hist_size;
  hmean.set(0, 0);
  hmeansq.set(0, 0);
  hvariance.set(0, 0);
  datasize = nr_xbins * nr_ybins;

  if ((nr_xbins == 0) || (nr_ybins == 0))
    return;

  vector2d num((double) nr_xbins, (double) nr_ybins);
  scale =  num / (maxv - minv);
  data = new ushort[datasize];

  memset(data, 0, sizeof(ushort) * datasize);
}



bool Histogram2D::add(vector2d v)
{
  // may need to use floor(x + 0.5) to center properely

  vector2d vrem = minv;
  vector2d vindx = ((v - minv) * scale);
  int xindx = (int) vindx.x;
  int yindx = (int) vindx.y;

  if ((xindx < 0) || (xindx >= (int) nr_xbins) ||
      (yindx < 0) || (yindx >= (int) nr_ybins)) {
    return false;
  }

  int indx = xindx + yindx * nr_xbins;

  // add to histogram
  data[indx]++;
  count++;

  // add the truncation - should mabye add 0.5 to center it
  v = vector2d((double) xindx + 0.5, (double) yindx + 0.5) / scale + minv;

  // udpate means and variances here
  double n = (double) count;
  hmean += (v - hmean) / n;
  hmeansq += (v * v - hmeansq) / n;
  hvariance = hmeansq - hmean * hmean;

  // now remove a value from the histogram if we have enough
  if (count > max_hist_size) {
    do {
      indx = rand() % datasize;
    } while (data[indx] == 0);
    data[indx]--;
    count--;
    yindx = indx / nr_xbins;
    xindx = indx % nr_xbins;
    vrem = (vector2d((double) xindx + 0.5, (double) yindx + 0.5) / scale) + minv;

    n = (double) count;
    hmean += (hmean - vrem) / n;
    hmeansq += (hmeansq - vrem * vrem) / n;
    hvariance = hmeansq - hmean * hmean;
  }

  return true;
}

inline ushort Histogram2D::e(vector2d v) 
{
  vector2d vindx = ((v - minv) * scale);
  int xindx = (int) vindx.x;
  int yindx = (int) vindx.y;

  if ((xindx < 0) || (xindx >= (int) nr_xbins) ||
      (yindx < 0) || (yindx >= (int) nr_ybins)) {
    return 0;
  }

  int indx = xindx + yindx * nr_xbins;
  return (data[indx]);
}

void Histogram2D::print(FILE *f)
{
  fprintf(f, "Histogram 1D: maxv (%f, %f) minv (%f, %f)\n",
	  maxv.x, maxv.y, minv.x, minv.y);
  fprintf(f, "\tnr_bins (%i, %i) maxcount %i, count %i\n",
	  nr_xbins, nr_ybins, max_hist_size, count);
  fprintf(f, "\t mean (%f, %f) variance (%f, %f) sdev (%f, %f)\n",
	  hmean.x, hmean.y, hvariance.x, hvariance.y, stdDev().x, stdDev().y);

  for (uint i = 0; i < datasize; i++) {
    fprintf(f, "%4i, ", data[i]);
    if ((i + 1) % 11 == 0)
      fprintf(f, "\n");
  }
}


#ifdef TEST_MAIN

int main(int argc, char *argv[])
{
#if 0
  double minv = -1000.0, maxv = 1000.0;

  //  Histogram1D hist(maxv, minv, 100, 1000);
  Histogram1D hist;

  hist.set(maxv, minv, 100, 1000);

  for (int i = 0; i < 10000; i++) {

    double v = ((((double) rand()) / (double) RAND_MAX) - 0.5) * maxv;
    fprintf(stdout, "%f\n", v);
    hist.add(v);
  }
  hist.print(stderr);
#else
  vector2d minv(-1000.0, -1000.0);
  vector2d maxv(1000.0, 1000.0);

  //  Histogram1D hist(maxv, minv, 100, 1000);
  Histogram2D hist;

  hist.set(maxv, minv, 10, 10, 100000);

  for (int i = 0; i < 100000; i++) {

    double vx = ((((double) rand()) / (double) RAND_MAX) - 0.5) * maxv.x * 2;
    double vy = ((((double) rand()) / (double) RAND_MAX) - 0.5) * maxv.y * 2;
    vector2d v(vx, vy);
    //    fprintf(stdout, "%f, %f\n", v.x, v.y);
    hist.add(v);
  }
  hist.print(stderr);
#endif
  return (0);
}

#endif
