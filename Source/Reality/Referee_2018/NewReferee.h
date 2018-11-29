//Immortals referee module for RefBox 2018
#include "../WorldState.h"
#include "../../Network/PracticalSocket.h"
#include "../../Network/Protobuf/referee2018.pb.h"
#include "../../Common/GameSetting.h"

#define MAX_REF_UDP_BUFF 1000

class NewReferee
{
private:
	bool our_color;

	std::string referee_UDP_Address;
	short refereePort;

	bool isConnected;

	UDPSocket * RefUDP;

	RefereeState* RefState;
	BallState* ballData;

    TVec2 LastPlacedBall;
    int move_hys;//For isKicked

	char incoming_buffer[MAX_REF_UDP_BUFF];
	int buffer_size;
	int command_CNT;

    SSL_Referee pSSLRef;
    SSL_Referee_Stage playTimeStage;
    SSL_Referee_Command commandState;
    
public:
    NewReferee ( GameSetting* settings,WorldState* State );
	bool connectToRefBox ( void );
	bool is_connect ( void );
	bool recieve ( void );
	bool isKicked ( TVec2 ballPos );
	void process ( void );
    
    int oppGK;
};