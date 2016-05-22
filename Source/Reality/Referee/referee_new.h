#include "network/netraw.h"
#include "protos/messages_robocup_ssl_referee.pb.h"
#include "../Vision/WorldState.h"

class NewReferee
{
private:
    bool initialized , connected;
    
	Net::UDP * RefUDP;
	std::string address;
	unsigned short port;
	bool color;
    
	char buffer[1000];
    int buffer_size;
    
    SSL_Referee pSSLRef;
    
public:
    NewReferee ( void );
	void Init ( const std::string & _address , const unsigned short _port , bool _color );
	bool Open ( void );
	bool Recieve ( void );
	void Process (WorldState & state);
    
    int oppGK;
};