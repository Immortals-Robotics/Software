// socket.cc
//
// Wrapper class for network communiation.  Handles both UDP
// (unreliable, unsequenced) and TCP (reliable and sequenced) datagram
// passing.
//
// Created by: Mike Bowling (mhb@cs.cmu.edu)
//             Adapated from simple-socket.{h,c} by Kwun Han
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

#include <stdio.h>
#include <iostream.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>

#include <assert.h>

#include <netdb.h>

#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/poll.h>

#include "macros.h"
#include "socket.h"

int Socket::inited = 0;

Socket::Socket(int _protocol = TCP, int _ack_period = 1, int _ack_loss = 4)
{
  pollfd pfd = {0,0,0};

  last_message = NULL;
  if (!inited) init();

  pollset.push_back(pfd);

  protocol = _protocol;
  ack_period = _ack_period;
  ack_loss = _ack_loss;
  status = NotConnected;
}

Socket::~Socket()
{
  if (status > 0) disconnect();
  if (last_message)
    free(last_message);
}

void Socket::set(int _protocol = TCP, int _ack_period = 1, int _ack_loss = 4)
{
  if (!inited) init();

  protocol = _protocol;
  ack_period = _ack_period;
  ack_loss = _ack_loss;
  status = NotConnected;
}

int Socket::connect_server(int bindingPort)
{
  if((fd = socket(AF_INET, (protocol == UDP ? 
			    SOCK_DGRAM : SOCK_STREAM), 0)) < 0) {
    perror("socket");
    return status = NotConnected;
  }
  
  sockaddr_in sockname;

  memset((char*) &sockname, 0, sizeof(sockname));

  sockname.sin_family = AF_INET;
  sockname.sin_addr.s_addr = htonl(INADDR_ANY);
  sockname.sin_port = htons(bindingPort);

  /* Kwun's original code
   */
  int one = 1;
  setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one));

  if(bind(fd, (struct sockaddr*) (&sockname), sizeof(sockname)) < 0) {
    perror("bind");
    status = NotConnected;
    return status;
  }

  if (protocol == TCP) {
    if(listen(fd, 5) < 0) {
      perror("listen");
      status = NotConnected;
      return status;
    }
  } else if (protocol == UDP) {
    last_message = (char *) malloc(sizeof(char) * MaxMessageSize);
    last_message_bytes = 0;
  }

  status = Server;
  return status;
}

int Socket::connect_client(const char* host, int port)
{
  struct sockaddr_in serv_addr;
  struct hostent *hostptr;

  if (!(hostptr = gethostbyname(host))) {
    status = NotConnected;
    return status;
  }

  memset((char *) &serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  memcpy((char *) &serv_addr.sin_addr, 
	 hostptr->h_addr_list[0], hostptr->h_length);
  serv_addr.sin_port = htons(port);

  if ((fd = socket(AF_INET, 
		   (protocol == UDP ? SOCK_DGRAM : SOCK_STREAM), 0)) < 0) {
    perror("socket");
    status = NotConnected;
    return status;
  }

  if (connect(fd, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) < 0) {
    perror("connect");
    close(fd);
    return status = NotConnected;
  }

  status = Client;

  if (protocol == UDP) { 
    ack(); 

    pollfd p = { fd, POLLIN, 0 };
    if (poll(&p, 1, 0) < 0 || p.revents & POLLERR) 
      return status = NotConnected;
  }

  return status;
}

static void handle_sigpipe(int sig)
{
}

void Socket::init()
{
  static struct sigaction pipeact;

  pipeact.sa_handler =  &handle_sigpipe;
  pipeact.sa_mask = sigset_t();
  pipeact.sa_flags = SA_RESTART;

  sigaction(SIGPIPE, &pipeact, 0);
}

int Socket::disconnect()
{
  if (status > 0) {
    if (protocol == TCP) {
      for(unsigned int i=0; i<tcp_clients.size(); i++) 
	close(tcp_clients[i]);
    }

    close(fd);
    return status = Disconnected;
  } else {
    return status;
  }
}

void Socket::remove_client(int cfd)
{
  int id,num;

  num = tcp_clients.size();
  id = 0;
  while((id < num) && (tcp_clients[id]!=cfd)) id++;

  if(id < num){
    pollset[id] = pollset[num-1];
    pollset.pop_back();
    tcp_clients[id] = tcp_clients[num-1];
    tcp_clients.pop_back();
  }
}

int Socket::recv(void* buf, int maxbytes)
{
  if (status <= 0) return 0;

  int nbytes;
  int rv = 0;

  // CLIENT SIDE
  // First check if there's already a message read off the socket.
  // Just read from the fd.  If TCP then read message size first.
  if (status == Client) {
    if (protocol == TCP) {
      ::recv(fd, &nbytes, sizeof(int), 0);
      if (maxbytes > nbytes) maxbytes = nbytes;
    }

    rv = ::recv(fd, buf, maxbytes, 0);
    if (rv == 0) disconnect();
    else if (rv < 0) disconnect();

    if (protocol == UDP && msgs_since_ack++ >= ack_period) ack();

  }

  // SERVER SIDE TCP
  // Find a client ready for reading and read off the message size and
  // then the message.
  else if (protocol == TCP) {
    int cfd = tcp_client_to_recv();

    nbytes = 0;
    rv = ::recv(cfd, &nbytes, sizeof(int), 0);
    if(rv == 0){
      remove_client(cfd);
      return(0);
    }

    if (maxbytes > nbytes) maxbytes = nbytes;
    rv = ::recv(cfd, buf, maxbytes, 0);
  }

  // SERVER SIDE UDP
  // For UDP just read from the fd.
  else {
    while(last_message_bytes == 0){
      udp_server_recv();
    }

    if (maxbytes > last_message_bytes) maxbytes = last_message_bytes;
    memcpy(buf, last_message, maxbytes);

    last_message_bytes = 0;
    rv = maxbytes;
  }

  return rv;
}

void Socket::udp_server_recv()
{
  struct sockaddr_in from;
  socklen_t fromlen = sizeof(from);

  last_message_bytes = recvfrom(fd, last_message, MaxMessageSize, 0, 
				(sockaddr *) &from, &fromlen);

  acked(from);
}

int Socket::send(const void* buf, int nbytes)
{
  if (status <= 0) return 0;

  int rv = 0;

  // CLIENT SIDE
  // Just write to the fd.  If TCP then write message size first.
  if (status == Client) {
    if (protocol == TCP) ::send(fd, &nbytes, sizeof(int), 0);
    rv = ::send(fd, buf, nbytes, 0);

    if (rv < 0) {
      if (errno == EPIPE || errno == ECONNREFUSED) disconnect(); 
      else { 
	perror("send (client)"); 
	exit(1); 
      }
    }
  } 

  // SERVER SIDE TCP
  // Write to the fd for each client, the message size then message.
  else if (protocol == TCP) {
    for(unsigned int i=0; i<tcp_clients.size(); i++) {
      ::send(tcp_clients[i], &nbytes, sizeof(int), 0);
      if (errno == EPIPE) { disconnect(); return 0; }

      rv = MIN(rv, ::send(tcp_clients[i], buf, nbytes, 0));
      if (errno == EPIPE) { disconnect(); return 0; }
    }

    if (rv < 0) { perror("send (server, TCP)"); exit(1); }

  } 

  // SERVER SIDE UDP
  // Write to the address of each client.
  else {
    for(unsigned int i=0; i<udp_clients.size(); i++) {
      rv = MIN(rv, ::sendto(fd, buf, nbytes, 0, 
			    (sockaddr *) &(udp_clients[i].a), 
			    sizeof(udp_clients[i].a)));
      if (errno == EINVAL || udp_clients[i].ttl-- < 0) {
	udp_clients.erase(udp_clients.begin() + i);
	rv = 0;
      }
    }

    if (rv < 0) perror("send (server, UDP)");
  }


  return rv;
}

void Socket::accept()
{
  pollfd pfd = {0,0,0};

  if (status != Server) return;

  if (protocol == TCP) {
    int cfd;

    if((cfd = ::accept(fd, NULL, 0)) < 0) {
      perror("accept");
      return;
    }

    tcp_clients.push_back(cfd);
    pollset.push_back(pfd);
  } else {
    int m = 10;
    while(last_message_bytes<=0 && m--){
      udp_server_recv();
    }
  }
}

int Socket::num_clients() 
{
  if (status != Server) return 0;
  if (protocol == TCP) return tcp_clients.size();
  else return udp_clients.size();
}

bool Socket::ready_for_recv()
{
  int num,changed;
  int i;

  if (status <= 0) return 0;

  if (status == Server && protocol == UDP && last_message_bytes > 0) 
    return true;

  if (status == Server && protocol == TCP) {
    num = tcp_clients.size();
    for(int i=0; i<num; i++) {
      pollset[i].fd = tcp_clients[i];
      pollset[i].events = POLLIN;
    }
    // printf("size = %d\n",tcp_clients.size());
  } else {
    num = 1;
    pollset[0].fd = fd;
    pollset[0].events = POLLIN;
  }

  while(true) {
    changed = poll(&pollset[0],num,0);
    if(!changed) return(false);

    // remove stale entries
    if (status == Server && protocol == TCP) {
      i = 0;
      while(i < num) {
        if(pollset[i].revents & (POLLERR|POLLHUP|POLLNVAL)) {
          remove_client(pollset[i].fd);
          num--;
        } else {
          i++;
        }
      }
    }

    // check for new data
    for(i=0; i<num; i++) {
      if(pollset[i].revents & POLLIN) {
        if (status == Server && protocol == UDP) {
          udp_server_recv();
          if (last_message_bytes > 0) return(true);
        } else {
          // printf("can read from %d\n",i);
          return(true);
        }
      }
    }
  }
}

bool Socket::ready_for_send()
{
  if (status <= 0) return 0;

  static struct timeval tv = {0, 0};
  static fd_set fdvar;
    
  int maxfd = 0;
  FD_ZERO(&fdvar);

  if (status == Server && protocol == TCP) {

    // mhb: Not sure this will do the right thing.  For a TCP server
    // you need to check wether each client is ready to receive a message.
    // Otherwise you risk blocking on a send.

    if (num_clients() == 0) return true;

    for(unsigned int i=0; i<tcp_clients.size(); i++) {
      FD_SET(tcp_clients[i], &fdvar);
      maxfd = MAX(maxfd, tcp_clients[i]);
    }

    return (select(maxfd+1, 0, &fdvar, 0, &tv) == (int) tcp_clients.size());
 
 } else {
    FD_SET(fd, &fdvar);
    maxfd = fd;
    return (select(maxfd+1, 0, &fdvar, 0, &tv) > 0);
  }

}

bool Socket::ready_for_accept()
{
  if (status == Server && protocol == TCP) {
    static struct timeval tv = {0, 0};
    static fd_set fdvar;
    
    FD_ZERO(&fdvar);
    FD_SET(fd, &fdvar);
    
    return (select(fd+1, &fdvar, 0, 0, &tv) > 0);
  } else 
    return (last_message_bytes > 0) ? false : ready_for_recv();
}

void Socket::ack()
{
  char ch;
  ch = 0;
  ::send(fd, &ch, 0, 0);
  msgs_since_ack = 0;
}

void Socket::acked(struct sockaddr_in &a)
{
  for(unsigned int i=0; i<udp_clients.size(); i++) {
    if (memcmp(&udp_clients[i].a, &a, sizeof(a)) == 0) {
      udp_clients[i].ttl = ack_period * ack_loss;
      return;
    }
  }

  udp_client c = {a, ack_period * ack_loss};
  udp_clients.push_back(c);
}

int Socket::tcp_client_to_recv()
{
  if (status != Server || protocol != TCP) return -1;

  static fd_set fdvar;
  int max_fd = 0;

  FD_ZERO(&fdvar);
  for(unsigned int i=0; i<tcp_clients.size(); i++) {
    FD_SET(tcp_clients[i], &fdvar);
    max_fd = MAX(max_fd, tcp_clients[i]);
  }

  if (select(max_fd+1, &fdvar, 0, 0, NULL) <= 0) return -1;

  for(unsigned int i=0; i<tcp_clients.size(); i++) {
    if (FD_ISSET(tcp_clients[i], &fdvar)) {
      // printf("recv from %d\n",i);
      return tcp_clients[i];
    }
  }

  return -1;
}

#if TEST_MAIN

#define PORT 32885

char messages[6][256] = { 
  "0 These are test messages.", 
  "1 They test wether the communication protocol is working.",
  "2 By sending these strings over the network,",
  "3 we can tell wether we get them out on the opposite end.",
  "4 The more strings the merrier!",
  "5 STOP",
};

int nmessages = 6;

void test_protocol(int protocol)
{
  Socket server(protocol);

  if (server.connect_server(PORT) != Socket::Server) exit(1);

  if (fork() == 0) {
    sleep(1);
    while(server.ready_for_accept()) server.accept();

    for(int i=0; i<nmessages; i++) {
      server.send(messages[i], strlen(messages[i]) + 1);
      usleep(0);
      while(server.ready_for_accept()) server.accept();
    }

    server.disconnect();
    exit(0);
  }

  Socket client[2] = { Socket(protocol), Socket(protocol) };
  
  char buffer[32767];
  int length;
  
  if (client[0].connect_client("localhost", PORT) != Socket::Client) {
    fprintf(stderr, "Client failed to connect!\n"); exit(1); 
  }
  
  if (client[1].connect_client("localhost", PORT) != Socket::Client) {
    fprintf(stderr, "Client failed to connect!\n"); exit(1); 
  }

  int i = 0;

  while(client[0].get_status() > 0 && client[1].get_status() > 0) {
    length = client[0].recv(buffer, 32767);
    if (length > 0 && strcmp(buffer, messages[i]) != 0) {
      fprintf(stderr, "Client read incorrect message!\n");
      fprintf(stderr, "Message %d: %s\n", i, buffer);
    }

    length = client[1].recv(buffer, 32767);
    if (length > 0 && strcmp(buffer, messages[i]) != 0) {
      fprintf(stderr, "Client read incorrect message!\n");
      fprintf(stderr, "Message %d: %s\n", i, buffer);
    }

    if (length > 0) i++;
    if (i == nmessages && protocol == Socket::UDP) break;
  }

  if (i != nmessages)
    fprintf(stderr, "Incorrect number of messages read!\n");
}

int main(int argc, char *argv[])
{
  fprintf(stderr, "Testing TCP protocol...\n");
  test_protocol(Socket::TCP);

  sleep(1);

  fprintf(stderr, "Testing UDP protocol...\n");
  test_protocol(Socket::UDP);
}

#endif

