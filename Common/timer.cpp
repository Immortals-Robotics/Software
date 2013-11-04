/*========================================================================
 Timer.h : Simple C++ wrapper for making interval timers
 ------------------------------------------------------------------------
 Copyright (C) 1999-2002  James R. Bruce
 School of Computer Science, Carnegie Mellon University
 ------------------------------------------------------------------------
 This software is distributed under the GNU General Public License,
 version 2.  If you do not have a copy of this licence, visit
 www.gnu.org, or write: Free Software Foundation, 59 Temple Place,
 Suite 330 Boston, MA 02111-1307 USA.  This program is distributed
 in the hope that it will be useful, but WITHOUT ANY WARRANTY,
 including MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 ========================================================================*/

#include "timer.h"


void Timer::start()  {gettimeofday(&tv1,NULL);}
void Timer::end()    {gettimeofday(&tv2,NULL);}
double Timer::time() {end();
	return((tv2.tv_sec - tv1.tv_sec) +
						  (tv2.tv_usec - tv1.tv_usec) / 1.0E6);}
double Timer::interval(){
	double t;
	gettimeofday(&tv2,NULL);
	t = time();
	tv1 = tv2;
	return(t);
}
