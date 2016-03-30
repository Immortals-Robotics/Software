#include "../../Network/PracticalSocket.h"
#include "../Vision/Protobuf/referee.pb.h"
#include "../Vision/WorldState.h"

class NewReferee
{
private:
    bool initialized , connected;
    
	UDPSocket * RefUDP;
	std::string address;
	unsigned short port;
	bool color;
    
	char buffer[1000];
    int buffer_size;
    
    SSL_Referee pSSLRef;
    
public:
    NewReferee ( void );
	void init ( const std::string & _address , const unsigned short _port , bool _color );
	bool connect ( void );
	bool recieve ( void );
	void process ( WorldState * state );
    
    int oppGK;
};