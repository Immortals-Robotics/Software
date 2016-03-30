#include "net_log.h"
#include <sstream>
#include <iostream>

    int NetLogger::sync()
    {
		// ensure NUL termination
		//overflow(0);
		//Send data
		string data = str();
        printf("%s", data.c_str());
        //data.insert(0, frame_id_str);
		//udpSock.sendTo(data.c_str(), data.length(), logger_ip, logger_port);
		// clear buffer
		str(std::string());
		return std::stringbuf::sync();
    }
	
NetLogger::NetLogger ( const string _address , const short _port )
	:logger_ip(_address),logger_port(_port),frame_id(0){}
    
void NetLogger::SetFrameID ( unsigned int id )
    {
        frame_id = id;
        //sprintf(frame_id_str, "%u\n", frame_id);
    }

unsigned int NetLogger::GetFrameID ( void )
{
    return frame_id;
}

void NetLogger::Init()
    {
        SetFrameID(0);
        std::cout.rdbuf(this);
    }