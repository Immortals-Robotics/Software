#pragma once
#include "game_state.h"
#include "network/netraw.h"
#include "math/Vector.h"
#include "../Vision/WorldState.h"

struct GameStatePacket{
	char cmd;                      // current referee command
	char cmd_counter;     // increments each time new command is set
	unsigned char goals_blue;      // current score for blue team
	unsigned char goals_yellow;    // current score for yellow team
	unsigned short time_remaining; // seconds remaining for current game stage (network byte order)
};


class Referee
{
private:
	bool initialized , connected;

	int move_hys;

	Net::UDP * RefUDP;
	std::string address;
	unsigned short port;
	bool color;

	TVec2 LastPlacedBall;

	GameState * gameState;

	GameStatePacket packet;
	char buffer[10];

public:
	Referee ( void );
	void Init ( const std::string & _address , const unsigned short _port , bool _color ); 
	bool Open ( void );
	bool Recieve ( void );
	void Process ( WorldState& state );
	bool IsKicked ( TVec2 ballPos );

};