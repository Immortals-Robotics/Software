//Immortals referee module for RefBox 2018
#include "../WorldState.h"
#include "../../Network/Protobuf/referee2018.pb.h"
#include "../../Common/GameSetting.h"
#include "../../Common/network/udp_client.h"
#include "../../Common/timer.h"

class NewReferee
{
private:
	bool our_color;

	std::unique_ptr<UdpClient> m_udp;

	RefereeState* RefState;
	BallState* ballData;

    TVec2 LastPlacedBall;
    int move_hys;//For isKicked

	Timer timer;

	int command_CNT;

    SSL_Referee pSSLRef;
    SSL_Referee_Stage playTimeStage;
    SSL_Referee_Command commandState;
    
public:
    NewReferee ( GameSetting* settings,WorldState* State );
	bool connectToRefBox ( void );
	bool isConnected( void );
	bool recieve ( void );
	bool isKicked ( TVec2 ballPos );
	void process ( void );
    
    int oppGK;
};