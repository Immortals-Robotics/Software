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

#ifndef __BALL_TRACKER_H__
#define __BALL_TRACKER_H__

#include <reality/net_vision.h>
#include "kalman.h"

class VTracker;

class BallTracker : private Kalman {
public:
  enum OccludeFlag { Visible, MaybeOccluded, Occluded };

private:
  bool _reset;

  double velocity_variance(const Matrix &x);
  bool check_for_collision(const Matrix &x, vector2d &cp, vector2d &cv,
			   int &team, int &robot);

  bool check_occlusion();

  void tick_occlusion(double dt);

  vector2d occluded_position(double time);
  vector2d occluded_velocity(double time);

  // Pointer up to the complete set of trackers... for collisions,
  // occlusions, and such.
  VTracker *tracker;

  // Occlusion Information
  OccludeFlag occluded;
  char occluding_team;
  char occluding_robot;
  vector2d occluding_offset;
  double occluded_last_obs_time;

  friend VTracker;

protected:
  virtual Matrix& f(const Matrix &x, Matrix &I); // noiseless dynamics
  virtual Matrix& h(const Matrix &x); // noiseless observation

  virtual Matrix& Q(const Matrix &x); // Covariance of propagation noise
  virtual Matrix& R(const Matrix &x); // Covariance of observation noise

  virtual Matrix& A(const Matrix &x); // Jacobian of f w.r.t. x
  virtual Matrix& W(const Matrix &x); // Jacobian of f w.r.t. noise
  virtual Matrix& H(const Matrix &x); // Jacobian of h w.r.t. x
  virtual Matrix& V(const Matrix &x); // Jacobian of h w.r.t. noise

public:
  BallTracker();
  virtual ~BallTracker() {}

  void set_tracker(VTracker *t) { tracker = t; }

  void reset() { _reset = true; }
  void reset(double timestamp, float state[4], float variances[16],
	     OccludeFlag occluded, 
	     char occluding_team, char occluding_robot,
	     vector2f occluding_offset);

  void observe(vraw obs, double timestamp);

  vector2d position(double time);
  vector2d velocity(double time);
  Matrix covariances(double time);

  bool collision(double time, int &team, int &robot);
};

#include "vtracker.h"

#endif
