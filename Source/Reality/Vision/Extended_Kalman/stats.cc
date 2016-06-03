/*
 * TITLE:        stats.cc
 *
 * PURPOSE:      This file encapsulates teh stats class
 *               for holding and calculating frame stats information
 *
 * WRITTEN BY:   Brett Browning
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

#include "stats.h"
#include "util.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "configreader.h"

CR_DECLARE(STATS_VISION_CONF);


FrameStats::FrameStats(double _window, bool _verbose)
{
  window = _window;
  verbose = _verbose;

  CR_SETUP(gui, STATS_VISION_CONF, CR_DOUBLE);

  // zero everything out
  memset(&mean, 0, sizeof(mean));
  memset(&sqmean, 0, sizeof(sqmean));
  memset(&variances, 0, sizeof(variances));
  memset(&maxframe, 0, sizeof(maxframe));
  memset(&maxacc, 0, sizeof(maxacc));
  memset(&avgacc, 0, sizeof(avgacc));
  memset(&acc, 0, sizeof(acc));
  memset(&missing, 0, sizeof(missing));
}

inline void FrameStats::AddVal(double &val, double add, double n)
{
  val = (n * val + add) / (n + 1.0);
}

inline void FrameStats::SubVal(double &val, double sub, double n)
{
  val = (n * val - sub) / (n - 1.0);
}

inline void FrameStats::AddVal(float &val, float add, double _n)
{
  float n = (float) _n;
  val = (n * val + add) / (n + 1.0);
}

inline void FrameStats::SubVal(float &val, float sub, double _n)
{
  float n = (float) _n;
  val = (n * val - sub) / (n - 1.0);
}

inline void FrameStats::AddVal(vector2f &val, vector2f add, double _n)
{
  float n = (float) _n;
  val = (val * n + add) / (n + 1.0);
}

inline void FrameStats::SubVal(vector2f &val, vector2f sub, double _n)
{
  float n = (float) _n;
  val = (val * n - sub) / (n - 1.0);
}


inline void FrameStats::AddVal(vraw &val, vraw add, double n)
{
  AddVal(val.conf, add.conf, n);
  AddVal(val.angle, add.angle, n);
  AddVal(val.pos, add.pos, n);
}

inline void FrameStats::SubVal(vraw &val, vraw sub, double n)
{
  SubVal(val.conf, sub.conf, n);
  SubVal(val.angle, sub.angle, n);
  SubVal(val.pos, sub.pos, n);
}

void FrameStats::AddVal(net_vframe &val, net_vframe add, double n)
{
  AddVal(val.ball.vision, add.ball.vision, n);
  AddVal(val.ball.state.x, add.ball.state.x, n);
  AddVal(val.ball.state.y, add.ball.state.y, n);
  AddVal(val.ball.state.vx, add.ball.state.vx, n);
  AddVal(val.ball.state.vy, add.ball.state.vy, n);

  for (int t = 0; t < NUM_TEAMS; t++) {
    for (int i = 0; i < MAX_TEAM_ROBOTS; i++) {
      AddVal(val.robots[t][i].vision, add.robots[t][i].vision, n);
      AddVal(val.robots[t][i].state.x, add.robots[t][i].state.x, n);
      AddVal(val.robots[t][i].state.y, add.robots[t][i].state.y, n);
      AddVal(val.robots[t][i].state.theta, add.robots[t][i].state.theta, n);
      AddVal(val.robots[t][i].state.vx, add.robots[t][i].state.vx, n);
      AddVal(val.robots[t][i].state.vy, add.robots[t][i].state.vy, n);
      AddVal(val.robots[t][i].state.vtheta, add.robots[t][i].state.vtheta, n);
      AddVal(val.robots[t][i].state.stuck, add.robots[t][i].state.stuck, n);
    }
  }
}

void FrameStats::SubVal(net_vframe &val, net_vframe sub, double n)
{
  SubVal(val.ball.vision, sub.ball.vision, n);
  SubVal(val.ball.state.x, sub.ball.state.x, n);
  SubVal(val.ball.state.y, sub.ball.state.y, n);
  SubVal(val.ball.state.vx, sub.ball.state.vx, n);
  SubVal(val.ball.state.vy, sub.ball.state.vy, n);
  for (int t = 0; t < NUM_TEAMS; t++) {
    for (int i = 0; i < MAX_TEAM_ROBOTS; i++) {
      SubVal(val.robots[t][i].vision, sub.robots[t][i].vision, n);
      SubVal(val.robots[t][i].state.x, sub.robots[t][i].state.x, n);
      SubVal(val.robots[t][i].state.y, sub.robots[t][i].state.y, n);
      SubVal(val.robots[t][i].state.theta, sub.robots[t][i].state.theta, n);
      SubVal(val.robots[t][i].state.vx, sub.robots[t][i].state.vx, n);
      SubVal(val.robots[t][i].state.vy, sub.robots[t][i].state.vy, n);
      SubVal(val.robots[t][i].state.vtheta, sub.robots[t][i].state.vtheta, n);
      SubVal(val.robots[t][i].state.stuck, sub.robots[t][i].state.stuck, n);
    }
  }
}

inline void FrameStats::SquareVal(vraw &val)
{
  val.pos.x *= val.pos.x;
  val.pos.y *= val.pos.y;
  val.angle *= val.angle;
  val.conf *= val.conf;
}

void FrameStats::SquareVal(net_vframe &val)
{
  SquareVal(val.ball.vision);
  val.ball.state.x *= val.ball.state.x;
  val.ball.state.y *= val.ball.state.y;
  val.ball.state.vx *= val.ball.state.vx;
  val.ball.state.vy *= val.ball.state.vy;

  for (int t = 0; t < NUM_TEAMS; t++) {
    for (int i = 0; i < MAX_TEAM_ROBOTS; i++) {
      SquareVal(val.robots[t][i].vision);
      val.robots[t][i].state.x *= val.robots[t][i].state.x;
      val.robots[t][i].state.y *= val.robots[t][i].state.y;
      val.robots[t][i].state.theta *= val.robots[t][i].state.theta;
      val.robots[t][i].state.vx *= val.robots[t][i].state.vx;
      val.robots[t][i].state.vy *= val.robots[t][i].state.vy;
      val.robots[t][i].state.vtheta *= val.robots[t][i].state.vtheta;
      val.robots[t][i].state.stuck *= val.robots[t][i].state.stuck;
    }
  }
}

void FrameStats::CalculateVariance(net_vframe &res, net_vframe &valsq, net_vframe &avg)
{
  net_vframe sqavg = avg;
  SquareVal(sqavg);
  
  res.ball.vision.conf = valsq.ball.vision.conf - sqavg.ball.vision.conf;
  res.ball.vision.angle = valsq.ball.vision.angle - sqavg.ball.vision.angle;
  res.ball.vision.pos = valsq.ball.vision.pos - sqavg.ball.vision.pos;

  res.ball.state.x = valsq.ball.state.x - sqavg.ball.state.x;
  res.ball.state.y = valsq.ball.state.y - sqavg.ball.state.y;
  res.ball.state.vx = valsq.ball.state.vx - sqavg.ball.state.vx;
  res.ball.state.vy = valsq.ball.state.vy - sqavg.ball.state.vy;

  for (int t = 0; t < NUM_TEAMS; t++) {
    for (int i = 0; i < MAX_TEAM_ROBOTS; i++) {
      res.robots[t][i].vision.conf = valsq.robots[t][i].vision.conf - 
	sqavg.robots[t][i].vision.conf;
      res.robots[t][i].vision.angle = valsq.robots[t][i].vision.angle 
	- sqavg.robots[t][i].vision.angle;
      res.robots[t][i].vision.pos = valsq.robots[t][i].vision.pos 
	- sqavg.robots[t][i].vision.pos;
      
      res.robots[t][i].state.x = valsq.robots[t][i].state.x 
	- sqavg.robots[t][i].state.x;
      res.robots[t][i].state.y = valsq.robots[t][i].state.y 
	- sqavg.robots[t][i].state.y;
      res.robots[t][i].state.theta = valsq.robots[t][i].state.theta 
	- sqavg.robots[t][i].state.theta;
      res.robots[t][i].state.vx = valsq.robots[t][i].state.vx 
	- sqavg.robots[t][i].state.vx;
      res.robots[t][i].state.vy = valsq.robots[t][i].state.vy 
	- sqavg.robots[t][i].state.vy;
      res.robots[t][i].state.vtheta = valsq.robots[t][i].state.vtheta 
	- sqavg.robots[t][i].state.vtheta;
      res.robots[t][i].state.stuck = valsq.robots[t][i].state.stuck 
	- sqavg.robots[t][i].state.stuck;
    }
  }
}

void FrameStats::CalculateMaxValues(net_vframe &val, net_vframe &add)
{
  // do ball first: only velocities
  vector2f v(val.ball.state.vx, val.ball.state.vy);
  vector2f v2(add.ball.state.vx, add.ball.state.vy);
  if (v.sqlength() < v2.sqlength()) {
    val.ball.state = add.ball.state;
    val.ball.vision.timestamp = add.timestamp;
    if (val.ball.vision.timestamp != val.timestamp) {
      if (add.timestamp<val.timestamp)
        val.timestamp = add.timestamp;
    } else {
      val.timestamp=add.timestamp;
      for (int t=0; t<NUM_TEAMS; t++) {
        for (int i=0; i<MAX_TEAM_ROBOTS; i++) {
	  if (val.robots[t][i].vision.timestamp<val.timestamp)
	    val.timestamp=val.robots[t][i].vision.timestamp;
	}
      }
    }
  }

  for (int t = 0; t < NUM_TEAMS; t++) {
    for (int i = 0; i < MAX_TEAM_ROBOTS; i++) {
      v.set(val.robots[t][i].state.vx, val.robots[t][i].state.vy);
      v2.set(add.robots[t][i].state.vx, add.robots[t][i].state.vy);

      if (v.sqlength() < v2.sqlength()) {
	val.robots[t][i].state = add.robots[t][i].state;
	if (val.ball.vision.timestamp==val.timestamp) {
          val.robots[t][i].vision.timestamp = add.timestamp;
	  if (add.timestamp<val.timestamp)
   	    val.timestamp = add.timestamp;
	} else {
	  if (val.robots[t][i].vision.timestamp == val.timestamp)
	    val.timestamp = add.timestamp;
          val.robots[t][i].vision.timestamp = add.timestamp;
	  for (int t=0; t<NUM_TEAMS; t++) {
	    for (int i=0; i<MAX_TEAM_ROBOTS; i++) {
	      if (val.robots[t][i].vision.timestamp<val.timestamp)
	        val.timestamp=val.robots[t][i].vision.timestamp;
	    }
	  }
	  if (add.timestamp < val.timestamp)
	    val.timestamp = add.timestamp;
	}
      }
    }
  }
}

//Calculates Accelation and returns it for frame acc in state.vx and state.vy for use of old code
void FrameStats::CalculateAcceleration()
{
  net_vframe prev;
  net_vframe curr;
  double time;

  if (vframes.size() > 1) {
    prev = vframes[vframes.size()-2];
    curr = vframes[vframes.size()-1];
    time = curr.timestamp-prev.timestamp;
    acc.timestamp = curr.timestamp;
    acc.ball.vision = curr.ball.vision;
    acc.ball.state.x = curr.ball.state.x;
    acc.ball.state.y = curr.ball.state.y;
    acc.ball.state.vx = (curr.ball.state.vx-prev.ball.state.vx)/time;
    acc.ball.state.vy = (curr.ball.state.vy-prev.ball.state.vy)/time;

    for (int t=0; t<NUM_TEAMS; t++) {
      for (int i=0; i<MAX_TEAM_ROBOTS; i++) {
        acc.robots[t][i].vision = curr.robots[t][i].vision;
        acc.robots[t][i].state.x = curr.robots[t][i].state.x;
	acc.robots[t][i].state.y = curr.robots[t][i].state.y;
	acc.robots[t][i].state.theta = curr.robots[t][i].state.theta;
	acc.robots[t][i].state.stuck = curr.robots[t][i].state.stuck;
	acc.robots[t][i].state.vx = (curr.robots[t][i].state.vx 
				     - prev.robots[t][i].state.vx) / time;
	acc.robots[t][i].state.vy = (curr.robots[t][i].state.vy 
				     - prev.robots[t][i].state.vy) / time;
	acc.robots[t][i].state.vtheta = (curr.robots[t][i].state.vtheta
					 - prev.robots[t][i].state.vtheta) / time;
      }
    }
  } else {
    acc.timestamp = window;
    acc.ball.state.vx=0;
    acc.ball.state.vy=0;
    for (int t=0; t<NUM_TEAMS; t++) {
      for (int i=0; i<NUM_TEAMS; i++) {
	acc.robots[t][i].state.vx=0;
	acc.robots[t][i].state.vy=0;
      }
    }
  }
}

// update the frame structure with the latest information
bool FrameStats::Update(net_vframe &latest)
{
  // check to make sure latest frame is new
  if (!vframes.empty() && (latest.timestamp <= vframes[vframes.size() - 1].timestamp))
    return (true);

  double n = (double) vframes.size();
  net_vframe sqframe = latest;
  SquareVal(sqframe);

  // add it to the list 
  vframes.push_back(latest);
  sqframes.push_back(sqframe);

  //Get Acceleration
  CalculateAcceleration();

  // add that to its list
  aframes.push_back(acc);
  
  // update our averages
  AddVal(mean, latest, n);
  AddVal(sqmean, sqframe, n);
  AddVal(avgacc, acc, n);
  CountMissing(missing, latest, n);

  // work out how many to pop off
  double tcutoff = latest.timestamp - window;

  // work out whether our maxframe values
  // needs to be changed
  CalculateMaxValues(maxframe, latest);
  CalculateMaxValues(maxacc, acc);
  if (maxframe.timestamp < tcutoff) {
    memset(&maxframe, 0, sizeof(maxframe));
    for (uint i = 0; i < vframes.size(); i++) 
      CalculateMaxValues(maxframe, vframes[i]);
  }

  if (maxacc.timestamp < tcutoff) {
    memset(&maxacc, 0, sizeof(maxacc));
    for (uint i = 0; i < aframes.size(); i++) 
      CalculateMaxValues(maxacc, aframes[i]);
  }

  // now remove whatever is old from the list
  // and correct values accordingly
  while (!vframes.empty() && (vframes[0].timestamp < tcutoff)) {
    n = (double) vframes.size();
    SubVal(mean, vframes[0], n);
    UnCountMissing(missing, vframes[0], n);
    vframes.pop_front();

    n = (double) aframes.size();
    SubVal(avgacc, aframes[0], n);
    aframes.pop_front();
    
    n = (double) sqframes.size();
    SubVal(sqmean, sqframes[0], n);
    sqframes.pop_front();
  }

  // update the variance calculation
  CalculateVariance(variances, sqmean, mean);
  return true;
}

void FrameStats::CountMissing(net_vframe &miss, net_vframe &latest, double n)
{
  // count ball missing values
  if ((latest.timestamp != latest.ball.vision.timestamp)
      ||(latest.ball.vision.conf < DVAR(STATS_VISION_CONF))) {
    missing.ball.vision.conf = (n * missing.ball.vision.conf + 1.0) / (n + 1.0);
  } else {
    missing.ball.vision.conf = (n * missing.ball.vision.conf) / (n + 1.0);
  }

  for (int t = 0; t < NUM_TEAMS; t++) {
    for (int i = 0; i < MAX_TEAM_ROBOTS; i++) {
      if ((latest.timestamp != latest.robots[t][i].vision.timestamp)
	  ||(latest.robots[t][i].vision.conf < DVAR(STATS_VISION_CONF))) {
	missing.robots[t][i].vision.conf = 
	  (n * missing.robots[t][i].vision.conf + 1.0) / (n + 1.0);
      } else {
	missing.robots[t][i].vision.conf = 
	  (n * missing.robots[t][i].vision.conf) / (n + 1.0);
      }
    }
  }
}

void FrameStats::UnCountMissing(net_vframe &miss, net_vframe &latest, double n)
{
  // count ball missing values
  if ((latest.timestamp != latest.ball.vision.timestamp)
      ||(latest.ball.vision.conf < DVAR(STATS_VISION_CONF))) {
    missing.ball.vision.conf = (n * missing.ball.vision.conf - 1.0) / (n - 1.0);
  } else {
    missing.ball.vision.conf = (n * missing.ball.vision.conf) / (n - 1.0);
  }

  for (int t = 0; t < NUM_TEAMS; t++) {
    for (int i = 0; i < MAX_TEAM_ROBOTS; i++) {
      if ((latest.timestamp != latest.robots[t][i].vision.timestamp)
	  ||(latest.robots[t][i].vision.conf < DVAR(STATS_VISION_CONF))) {
	missing.robots[t][i].vision.conf = 
	  (n * missing.robots[t][i].vision.conf - 1.0) / (n - 1.0);
      } else {
	missing.robots[t][i].vision.conf = 
	  (n * missing.robots[t][i].vision.conf) / (n - 1.0);
      }
    }
  }
}

void FrameStats::Print(FILE *f)
{
  if (verbose) {
    fprintf(f, "Frame stats is based on %i records\n", vframes.size());
    fprintf(f, "Ball: raw: c %1.4f [%1.6f]\n",
	    mean.ball.vision.conf, variances.ball.vision.conf);
    fprintf(f, "\t: pos (%1.4f, %1.4f) [%1.4f, %1.4f]\n", 
	    mean.ball.vision.pos.x, mean.ball.vision.pos.y,
	    variances.ball.vision.pos.x, variances.ball.vision.pos.y);
    fprintf(f, "\tKBF: pos (%1.4f, %1.4f) [%1.4f %1.4f] \n", 
	    mean.ball.state.x, mean.ball.state.y, 
	    variances.ball.state.x, variances.ball.state.y);

    vector2f v(mean.ball.state.vx, mean.ball.state.vy);
    vector2f vv(variances.ball.state.vx, variances.ball.state.vy);
    fprintf(f, "\t: vel (%1.1f %1.1f) sp %1.1f [%1.1f %1.1f] %1.1f\n", 
	    v.x, v.y, v.length(), vv.x, vv.y, vv.length());
    
    v.set(maxframe.ball.state.vx, maxframe.ball.state.vy);
    fprintf(f, "\tmax: vel (%1.1f %1.1f) sp %1.1f mm/s @ time %f\n", 
	    v.x, v.y, v.length(), maxframe.ball.vision.timestamp);

    
    for (int t = 0; t < NUM_TEAMS; t++) {
      for (int i = 0; i < MAX_TEAM_ROBOTS; i++) {
	fprintf(f, "robot t %i i %i: raw: c %1.4f [%1.6f]\n", t, i,
		mean.robots[t][i].vision.conf, variances.robots[t][i].vision.conf);
	fprintf(f, "\t: pos (%1.2f, %1.2f, %1.4f) [%1.2f, %1.2f %1.4f]\n", 
		mean.robots[t][i].vision.pos.x, mean.robots[t][i].vision.pos.y,
		mean.robots[t][i].vision.angle,
		variances.robots[t][i].vision.pos.x, variances.robots[t][i].vision.pos.y,
		variances.robots[t][i].vision.angle);
	fprintf(f, "\tKBF: pos (%1.1f, %1.1f, %1.4f) [%1.1f %1.1f %1.4f] \n", 
		mean.robots[t][i].state.x, mean.robots[t][i].state.y, 
		mean.robots[t][i].state.theta,
		variances.robots[t][i].state.x, variances.robots[t][i].state.y,
		variances.robots[t][i].state.theta);

	v.set(mean.robots[t][i].state.vx, maxframe.robots[t][i].state.vy);
	vv.set(variances.robots[t][i].state.vx, variances.robots[t][i].state.vy);

	fprintf(f, "\t: vel (%1.1f %1.1f %1.4f) sp %1.1f [%1.1f %1.1f %1.4f]\n", 
		v.x, v.y, mean.robots[t][i].state.vtheta, v.length(),
		vv.x, vv.y, variances.robots[t][i].state.vtheta);

	// max values
	v.set(maxframe.robots[t][i].state.vx, maxframe.robots[t][i].state.vy);
	fprintf(f, "\tmax: %f %f %f mm/s\n", v.x, v.y, v.length());
      }
    }

  } else {

    // means first
    fprintf(f, "%f %f %f %f %f %f %f %f ",
	    mean.ball.vision.conf, mean.ball.vision.angle, 
	    mean.ball.vision.pos.x, mean.ball.vision.pos.y,
	    mean.ball.state.x, mean.ball.state.y, 
	    mean.ball.state.vx, mean.ball.state.vy);

    // vairances
    fprintf(f, "%f %f %f %f %f %f %f %f ",
	    variances.ball.vision.conf, variances.ball.vision.angle, 
	    variances.ball.vision.pos.x, variances.ball.vision.pos.y,
	    variances.ball.state.x, variances.ball.state.y, 
	    variances.ball.state.vx, variances.ball.state.vy);

    // max values
    vector2f v(maxframe.ball.state.vx, maxframe.ball.state.vy);
    fprintf(f, "%f %f %f\n", v.x, v.y, v.length());
    
    for (int t = 0; t < NUM_TEAMS; t++) {
      for (int i = 0; i < MAX_TEAM_ROBOTS; i++) {
	// means first
	fprintf(f, "%f %f %f %f %f %f %f %f %f %f %f",
		mean.robots[t][i].vision.conf, mean.robots[t][i].vision.angle, 
		mean.robots[t][i].vision.pos.x, mean.robots[t][i].vision.pos.y,
		mean.robots[t][i].state.x, mean.robots[t][i].state.y, 
		mean.robots[t][i].state.theta,
		mean.robots[t][i].state.vx, mean.robots[t][i].state.vy,
		mean.robots[t][i].state.vtheta, mean.robots[t][i].state.stuck);

	// vairances
	fprintf(f, "%f %f %f %f %f %f %f %f %f %f %f",
		variances.robots[t][i].vision.conf, variances.robots[t][i].vision.angle, 
		variances.robots[t][i].vision.pos.x, variances.robots[t][i].vision.pos.y,
		variances.robots[t][i].state.x, variances.robots[t][i].state.y, 
		variances.robots[t][i].state.theta, 
		variances.robots[t][i].state.vx, variances.robots[t][i].state.vy,
		variances.robots[t][i].state.vtheta, variances.robots[t][i].state.stuck);
	
	// max values
	vector2f v(maxframe.robots[t][i].state.vx, maxframe.robots[t][i].state.vy);
	fprintf(f, "%f %f %f\n", v.x, v.y, v.length());
      }
    }
  }
}



