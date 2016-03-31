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
    
	RefUDP = new UDPSocket ( );
    
	initialized = true;
}

bool NewReferee::connect ( void )
{
	if ( !initialized )
		return false;
    
	try
	{
		RefUDP -> setLocalPort ( port );
		RefUDP -> joinGroup ( address );
		connected = true;
		return true;
	}catch(...)
	{
		connected = false;
		return false;
	}
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
    
	buffer_size = RefUDP -> recv ( buffer , 1000 );
	return true;
}