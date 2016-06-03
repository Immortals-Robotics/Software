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

#ifndef __ROBOT_TRACKER_H__
#define __ROBOT_TRACKER_H__

#include <reality/net_vision.h>
#include "kalman.h"

class RobotTracker : private Kalman {
private:
  int type;
  double latency;

  int reset_on_obs;

  struct rcommand {
    double timestamp;
    vector3d vs;
  };

  deque<rcommand> cs; // Velocity commands

  rcommand get_command(double time);

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
  RobotTracker(int type = ROBOT_TYPE_NONE,
	       double _latency = LATENCY_DELAY);
  virtual ~RobotTracker() {}

  void set_type(int _type) { type = _type; reset(); }

  void reset() { reset_on_obs = true; }
  void reset(double timestamp, float state[6]);

  void observe(vraw obs, double timestamp);
  void command(double timestamp, vector3d vs);

  vector2d position(double time);
  vector2d velocity(double time);
  vector2d velocity_raw(double time);
  double direction(double time);
  double angular_velocity(double time);

  double stuck(double time);
};

#endif
