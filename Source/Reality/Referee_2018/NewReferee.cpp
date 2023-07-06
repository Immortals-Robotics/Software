#include "NewReferee.h"
#include "../../Common/distance.h"

NewReferee::NewReferee ( GameSetting* settings,WorldState* state ):isConnected(false)
{
	if(!settings){
		cout<<"NewReferee: \"setting\" is NULL"<<endl;
	}
	if(!state){
		cout<<"NewReferee: \"state\" is NULL"<<endl;
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
//		cout<<"HAS POSITION!!!!!"<<endl;
//		cout<<"BALL TARGET POSITION IS:"<<pSSLRef.designated_position().x()<<'_'<<pSSLRef.designated_position().y()<<endl;
        RefState->placeBallTargetPosition = Vec2(pSSLRef.designated_position().x(),pSSLRef.designated_position().y());
	}
//	else
//		cout<<"no new packet received"<<endl;

	if(command_CNT != pSSLRef.command_counter()) {//Update only when there is a new command
		command_CNT = pSSLRef.command_counter();

        LastPlacedBall = ballData->Position;

		if (our_color == TEAM_BLUE)
			RefState->oppGK = pSSLRef.yellow().goalie();
		else
			RefState->oppGK = pSSLRef.blue().goalie();

        move_hys = 0;//TODO maybe it needs to be commented


//		cout << "command: " << pSSLRef.command() << endl;
//		cout << "command_CNT: " << pSSLRef.command_counter() << endl;
	}

    RefState->State->transition ( pSSLRef.command() , isKicked(ballData->Position) );
//    if ( isKicked(ballData->Position) )
//    	cout << "kicked" << endl;

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
//	cout<<"the distance: "<<DIS ( ballPos , LastPlacedBall )<<endl;
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