#include "referee.h"
#include "../../Common/distance.h"

Referee::Referee ( void )
{
	initialized = false;
	connected = false;

	gameState = new GameState ( );

	LastPlacedBall = Vec2 ( 0.0f );

	packet.cmd_counter = 0;

	move_hys = 0;
}

void Referee::init ( const std::string & _address , const unsigned short _port , bool _color )
{
	address = _address;
	port = _port;
	color = _color;

	RefUDP = new UDPSocket ( );

	gameState -> init ( color );

	initialized = true;
}

bool Referee::connect ( void )
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
void Referee::process ( WorldState * state )
{
	packet.cmd = buffer[0];
	packet.goals_blue = buffer[2];
	packet.goals_yellow = buffer[3];
	packet.time_remaining = 256*buffer[4] + buffer[5];

	//cout << (int)buffer[1] << endl;

	if ( packet.cmd_counter != buffer[1] )
	{
		LastPlacedBall = state -> ball.Position;
		packet.cmd_counter = buffer[1];
		cout << "new cmd" << endl;
	}
	//cout << packet.cmd << endl;

	gameState -> transition ( packet.cmd , IsKicked(state->ball.Position) );
	//if ( IsKicked(state->ball.Position) )
	//	cout << "kicked" << endl;

	state -> refereeState.counter = packet.cmd_counter;
	//cout << (int)packet.cmd_counter << endl;
	state -> refereeState.goals_blue = packet.goals_blue;
	state -> refereeState.goals_yellow = packet.goals_yellow;
	state -> refereeState.time_remaining = packet.time_remaining;

	state -> refereeState.State = gameState;
}

bool Referee::IsKicked ( TVec2 ballPos )
{
	int requiredHys = 5;
    float requiredDis = 50.0f;
	if (gameState) {
		if (gameState->ourRestart()) {
			requiredHys = 5;
            requiredDis = 150.0f;
		}
	}
	if ( DIS ( ballPos , LastPlacedBall ) > requiredDis )
	{
		move_hys ++;
	}
	if ( move_hys >= requiredHys )
	{
		move_hys = 0;
		return true;
	}
	return false;
}

bool Referee::recieve ( void )
{
	if ( ( !initialized ) || ( !connected ) )
		return false;

	if ( RefUDP -> recv ( buffer , 10 ) == 6 )
	{
		return true;
	}
	return false;
}