#include <string>
#include <sstream>
#include <iostream>
#include "../Network/PracticalSocket.h"

class NetLogger : public std::stringbuf
{
protected:
	string logger_ip;
	short logger_port;
	UDPSocket udpSock;
	
    unsigned int frame_id;
    char frame_id_str[100];
    
    virtual int sync();
	
public:
	NetLogger ( const string _address , const short _port );
    
    void SetFrameID ( unsigned int id );
    unsigned int GetFrameID ( void );
    
    void Init();
};