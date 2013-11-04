#include "PracticalSocket.h"
#include "net_log.h"
#include <sstream>
#include <iostream>

class NetLogger : public std::stringbuf
{
protected:
	string logger_ip;
	short logger_port;
	UDPSocket udpSock;
	
    virtual int sync()
    {
		// ensure NUL termination
		//overflow(0);
		//Send data
		string data = str();
		udpSock.sendTo(data.c_str(), data.length(), logger_ip, logger_port);
		printf(data.c_str());
		// clear buffer
		str(std::string());
		return std::stringbuf::sync();
    }
	
public:
	NetLogger ( const string _address , const short _port )
	:logger_ip(_address),logger_port(_port){}
};

void init_net_log ( const string address , const short port )
{
	NetLogger* netLogger = new NetLogger ( address , port );
	std::cout.rdbuf(netLogger);
}