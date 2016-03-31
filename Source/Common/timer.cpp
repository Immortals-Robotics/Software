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

using namespace std::chrono;

void Timer::start() { tv1 = high_resolution_clock::now(); }
void Timer::end()   { tv2 = high_resolution_clock::now(); }

double Timer::time() 
{
	end();
	auto time_span = duration_cast<duration<double>>(tv2 - tv1);
	return time_span.count();
}

double Timer::interval(){
	double t;
	tv2 = high_resolution_clock::now();
	t = time();
	tv1 = tv2;
	return(t);
}
