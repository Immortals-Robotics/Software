#include "referee_new.h"

NewReferee::NewReferee ( void )
{
	initialized = false;
	connected = false;
    
    buffer_size = 0;
    
    oppGK = -1;
}

void NewReferee::init ( const std::string & _address , const unsigned short _port , bool _color )
{
	address = _address;
	port = _port;
	color = _color;
    
	RefUDP = new Net::UDP ( );
    
	initialized = true;
}

bool NewReferee::connect ( void )
{
	if ( !initialized )
		return false;
    
	if (!RefUDP->open(port, true, false, true))
	{
		fprintf(stderr, "Unable to open UDP network port: %d\n", port);
		fflush(stderr);
		connected = false;
		return false;
	}

	Net::Address multiaddr, interf;
	multiaddr.setHost(address.c_str(), port);

	interf.setAny();

	if (!RefUDP->addMulticast(multiaddr, interf)) {
		fprintf(stderr, "Unable to setup UDP multicast\n");
		fflush(stderr);
		connected = false;
		return(false);
	}

	connected = true;

	return true;
}
void NewReferee::process ( WorldState * state )
{
    if ( !pSSLRef.ParseFromArray(buffer, buffer_size) )
    {
        return;
    }
    
    if ( color == TEAM_BLUE )
        oppGK = pSSLRef.yellow().goalie();
    else
        oppGK = pSSLRef.blue().goalie();
    
    state -> oppGK = this -> oppGK;
}

bool NewReferee::recieve ( void )
{
	if ( ( !initialized ) || ( !connected ) )
		return false;
	
	Net::Address src;
	buffer_size = RefUDP -> recv ( buffer , 1000 , src );
	return buffer_size > 0;
}