#include "NewReferee.h"
#include "../../Common/distance.h"

NewReferee::NewReferee ( GameSetting* settings,WorldState* state ):isConnected(false)
{
	if(!settings){
		std::cout<<"NewReferee: \"setting\" is NULL"<< std::endl;
	}
	if(!state){
		std::cout<<"NewReferee: \"state\" is NULL"<< std::endl;
	}
	referee_UDP_Address = settings->referee_UDP_Address;
	refereePort = settings->refereePort;
	our_color = settings->our_color;
	buffer_size = -1;
	command_CNT = -1;

	RefState = state->refereeState;
	ballData = &(state->ball);

	RefState->State->init(our_color);
}

bool NewReferee::connectToRefBox ( void )
{

	RefUDP = new UDPSocket ( );

	try
	{
		RefUDP -> setLocalPort ( refereePort );
		RefUDP -> joinGroup ( referee_UDP_Address );
		isConnected = true;
		return true;
	}catch(...)
	{
		isConnected = false;
		return false;
	}
}

void NewReferee::process ()
{
    if ( !pSSLRef.ParseFromArray(incoming_buffer, buffer_size) )
        return;
	if(pSSLRef.has_designated_position()){
//		std::cout<<"HAS POSITION!!!!!"<<std::endl;
//		std::cout<<"BALL TARGET POSITION IS:"<<pSSLRef.designated_position().x()<<'_'<<pSSLRef.designated_position().y()<<std::endl;
        RefState->placeBallTargetPosition = Vec2(pSSLRef.designated_position().x(),pSSLRef.designated_position().y());
	}
//	else
//		std::cout<<"no new packet received"<<std::endl;

	if(command_CNT != pSSLRef.command_counter()) {//Update only when there is a new command
		command_CNT = pSSLRef.command_counter();

        LastPlacedBall = ballData->Position;

		if (our_color == TEAM_BLUE)
			RefState->oppGK = pSSLRef.yellow().goalie();
		else
			RefState->oppGK = pSSLRef.blue().goalie();

        move_hys = 0;//TODO maybe it needs to be commented


//		std::cout << "command: " << pSSLRef.command() << std::endl;
//		std::cout << "command_CNT: " << pSSLRef.command_counter() << std::endl;
	}

    RefState->State->transition ( pSSLRef.command() , isKicked(ballData->Position) );
//    if ( isKicked(ballData->Position) )
//    	std::cout << "kicked" << std::endl;

}

bool NewReferee::isKicked ( TVec2 ballPos )
{
	int requiredHys = 5;
	float requiredDis = 50.0f;
	if (RefState) {
		if (RefState->State->ourRestart()) {
			requiredHys = 5;
			requiredDis = 150.0f;
		}
	}
//	std::cout<<"the distance: "<<DIS ( ballPos , LastPlacedBall )<<std::endl;
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

bool NewReferee::recieve ( void )
{
	if ( !isConnected )
		return false;

	try
	{
		buffer_size = RefUDP->recv(incoming_buffer, MAX_REF_UDP_BUFF);
	}
	catch (SocketException exception)
	{
		return false;
	}
	return true;
}