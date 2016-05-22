#include "referee_new.h"

NewReferee::NewReferee ( void )
{
	initialized = false;
	connected = false;
    
    buffer_size = 0;
    
    oppGK = -1;
}

void NewReferee::Init ( const std::string & _address , const unsigned short _port , bool _color )
{
	address = _address;
	port = _port;
	color = _color;
    
	RefUDP = new Net::UDP ( );
    
	initialized = true;
}

bool NewReferee::Open ( void )
{
	if (!initialized)
		return false;

	if (!RefUDP->open(port, true, true))
	{
		connected = false;
		return false;
	}
	Net::Address multiaddr, interf;
	multiaddr.setHost(address.c_str(), port);
	interf.setAny();

	if (!RefUDP->addMulticast(multiaddr, interf)) {
		connected = false;
		return false;
	}

	connected = true;
	return true;
}
void NewReferee::Process (WorldState & state)
{
    if ( !pSSLRef.ParseFromArray(buffer, buffer_size) )
    {
        return;
    }
    
    if ( color == TEAM_BLUE )
        oppGK = pSSLRef.yellow().goalie();
    else
        oppGK = pSSLRef.blue().goalie();
    
    state.oppGK = this -> oppGK;
}

bool NewReferee::Recieve ( void )
{
	Net::Address src;
	if ( ( !initialized ) || ( !connected ) )
		return false;
    
	buffer_size = RefUDP -> recv ( buffer , 1000 , src);
	return true;
}