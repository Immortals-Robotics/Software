/*========================================================================
    Serial.h : Class wrapper for Serial I/O on Linux
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

#ifndef __SERIAL_H__
#define __SERIAL_H__

#include <unistd.h>

class Serial{
  int fd;
public:
  Serial() {fd=0;}
  ~Serial() {Close();}

  int get_fd(void) {
    return fd;
  }

  bool Open(const char *device,int baud,int flags=0);
  void Close();

  bool ready_for_recv(void);

  int Read(void *buf,int size)
    {return(::read(fd,buf,size));}
  int Write(void *buf,int size);
};

#endif
