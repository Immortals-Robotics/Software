// socket.h
//
// Wrapper class for network communiation.  Handles both UDP
// (unreliable, unsequenced) and TCP (reliable and sequenced) datagram
// passing.
//
// Created by: Mike Bowling (mhb@cs.cmu.edu)
//             Adapated from simple-socket.{h,c} by Kwun Han
//
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

#ifndef __socket_h__
#define __socket_h__

#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>

#include <netdb.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/poll.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <vector>
using namespace std;

//
// Socket class for a basic client/server model.
//
// The server uses connect_server() to start listening on a port.
// Clients connect with connect_client().
//
// Both clients and servers may freely use read() and write() to
// communicate.  For the server write() will send the message to all
// clients, and read() will read a message from some client.
//
// Since UDP is connectionless clients are determined to be those that
// have sent the server a message "recently".  UDP clients therefore
// ack() after each received message from the server, which sends a
// zero-length message which does not get passed out via the read()
// message.
//
// The accept() and ready_for_accept() methods have obvious meanings
// for TCP servers.  For UDP servers they should be used when the
// server doesn't read from its clients, since regularly reading off
// the read() port will keep clients in the list of connected clients.
// What these methods do will be to read its port dropping all the
// messages.  For generic use (i.e., when the protocol is not known),
// one should always read everything off the server's socket before
// checking ready_for_accept(), which will save messages from getting
// dropped.
// 
// NOTICE: Currently there is no way to know which client sent which
// message, nor to send message to specific clients.
//
// NOTICE: This is not specifically designed to handle streaming
// communication.
//

class Socket {
public:
  enum { NotConnected = -1,  // Never connected.
	 Disconnected = 0,   // Connection was dropped.
         Client       = 1,   // Connected as a client.
         Server       = 2    // Connected as a server.
  };

  enum { UDP, TCP };

  static const int MaxMessageSize = 32767;

protected:
  // Generic Socket Information
  int protocol;
  int status;
  int fd;

  // UDP ack parameters
  int ack_period, ack_loss;
  int msgs_since_ack;

  // Client structure and lists
  struct udp_client {
    struct sockaddr_in a;
    int ttl;
  };

  vector<int> tcp_clients;
  vector<udp_client> udp_clients;
  vector<pollfd> pollset;

  // Generic initialization
  static int inited;
  static void init();

  void ack();
  void acked(struct sockaddr_in &a);

  // UDP server reading (to handle and skip ack messages) 
  char *last_message;
  int last_message_bytes;

  void udp_server_recv();

public:
  Socket(int _protocol = TCP, int _ack_period = 1, int _ack_loss = 4);
  ~Socket();

  void set(int _protocol = TCP, int _ack_period = 1, int _ack_loss = 4);

  // Returns the new status.
  int connect_server(int binding_port);
  int connect_client(const char *host, int binding_port);
  int disconnect();
  void remove_client(int cfd);

  // return the file descriptor
  int get_fd(void) {
    return (fd);
  }

  int get_status() { return status; }

  // Returns the number of bytes read or written.
  int recv(void *buf, int maxbytes);
  int send(const void *buf, int nbytes);
  int recv_type(void *buf, int maxbytes, char &type) {
    int rv = recv(buf, maxbytes); type = ((char *) buf)[0]; return rv; }

  // Server specific methods
  void accept();
  int num_clients();

  // Returns if the socket can be written, read, or accepted without blocking.
  bool ready_for_recv();
  bool ready_for_send();
  bool ready_for_accept();

private:

  int tcp_client_to_recv();
};

#endif 
