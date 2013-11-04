/*========================================================================
    Serial.cc : Class wrapper for Serial I/O on Linux
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

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <fcntl.h>

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <termios.h>
#include <string.h>

#include "serial.h"

// #define RADIO_DEVICE "/dev/ttyS0"

#define BAUD_RATES 20
const unsigned baud_rates[BAUD_RATES][2] = {
  {      B0,      0 },
  {     B50,     50 },
  {     B75,     75 },
  {    B110,    110 },
  {    B134,    134 },
  {    B150,    150 },
  {    B200,    200 },
  {    B300,    300 },
  {    B600,    600 },
  {   B1200,   1200 },
  {   B1800,   1800 },
  {   B2400,   2400 },
  {   B4800,   4800 },
  {   B9600,   9600 },
  {  B19200,  19200 },
  {  B38400,  38400 },
  {  B57600,  57600 },
  { B115200, 115200 },
  { B230400, 230400 }
};

speed_t baud_rate_to_flag(unsigned speed)
{
  int i;

  for(i=0; i<BAUD_RATES; i++){
    if(baud_rates[i][1] == speed) return(baud_rates[i][0]);
  }

  return(0);
}

unsigned baud_flag_to_rate(speed_t s)
{
  int i;

  for(i=0; i<BAUD_RATES; i++){
    if(baud_rates[i][0] == s) return(baud_rates[i][1]);
  }

  return(0);
}

bool Serial::Open(const char *device,int baud,int flags)
{
  struct termios tio;
  speed_t bf;
  int baud_in,baud_out;

  // open device
  if(!flags) flags = O_RDWR | O_SYNC;
  fd = ::open(device,flags);
  if(fd < 0) return(false);

  // set the parameters we need
  tcgetattr(fd,&tio);
  cfmakeraw(&tio);                      // set up for binary I/O
  tio.c_cflag &= ~(CRTSCTS|IXON|IXOFF); // disable flow control

  // set serial port speed
  bf = baud_rate_to_flag(baud);
  cfsetispeed(&tio,bf);
  cfsetospeed(&tio,bf);
  tcsetattr(fd,TCSANOW,&tio);

  // test what actually got set
  memset(&tio,0,sizeof(tio));
  tcgetattr(fd,&tio);
  baud_in  = baud_flag_to_rate(cfgetispeed(&tio));
  baud_out = baud_flag_to_rate(cfgetospeed(&tio));

  return(baud_in==baud && baud_out==baud);
}

bool Serial::ready_for_recv(void)
{
  fd_set rfds;
  struct timeval tv;

  FD_ZERO(&rfds);
  FD_SET(fd, &rfds);
  tv.tv_sec = 0; tv.tv_usec = 0;
  return (select(1 + fd, &rfds, NULL, NULL, &tv));
}

void Serial::Close()
{
  if(fd){
    ::close(fd);
    fd = 0;
  }
}

int Serial::Write(void *buf,int size)
{
  int n;

  // tcflush(fd,TCOFLUSH);
  // tcdrain(fd);
  n = ::write(fd,buf,size);
  // tcdrain(fd);
  // usleep(1000*50);

  return(n);
}
