#include "vision.h"

#include "FilteredObject.h"

#include <fstream>
using namespace std;
extern ofstream* nikRastKarde;

VisionSetting * _visionSetting ( bool _color , std::string _UDP_Adress , short _LocalPort , std::string _GUI_Adress , short _GUIPort , bool _use_camera0 , bool _use_camera1 )
{
	VisionSetting * ans = new VisionSetting;

	ans -> color = _color;
	ans -> UDP_Adress = _UDP_Adress;
	ans -> LocalPort = _LocalPort;
	ans -> GUI_Adress = _GUI_Adress;
	ans -> GUIPort = _GUIPort;
	ans -> use_camera[0] = _use_camera0;
	ans -> use_camera[1] = _use_camera1;

	return ans;
}

VisionModule::VisionModule ( VisionSetting * _setting ) : connected ( false ) 
{
	if ( _setting )
	{
		setting = _setting;

		connectToVisionServer ( setting -> UDP_Adress , setting -> LocalPort );
	}

	GUIUDP = new UDPSocket ( );

	packet_recieved[0] = false;
	packet_recieved[1] = false;

	ballBufferIndex = 0;

	//lastShootPosition = Vec2 ( 0.0f );
	//prevBallVel = Vec2 ( 0.0f );

	for ( int i = 0 ; i < BALL_BUFFER_FRAMES ; i ++ )
	{
		ballBufferX[i] = 0.0f;
		ballBufferY[i] = 0.0f;
	}

	lastRawBall.set_x ( 0.0f );
	lastRawBall.set_y ( 0.0f );

	ball_kalman.initialize ( "../../ballFilterFast.txt" , "../../ballFilterSlow.txt" );
	
	for ( int i = 0 ; i < MAX_ROBOTS; i++ )
	{
		robot_kalman[0][i].initialize ( "../../ballFilterFast.txt" , "../../ballFilterSlow.txt" );
		robot_kalman[1][i].initialize ( "../../ballFilterFast.txt" , "../../ballFilterSlow.txt" );
		rawAngles[0][i] = 0.0f;
		rawAngles[1][i] = 0.0f;
	}
	
	//InitANN();
	
	ball_not_seen = MAX_BALL_NOT_SEEN + 1;
	for ( int i = 0 ; i < MAX_ROBOTS ; i ++ )
	{
		robot_not_seen[0][i] = MAX_ROBOT_NOT_SEEN + 1;
		robot_not_seen[1][i] = MAX_ROBOT_NOT_SEEN + 1;
	}
}
VisionModule::~VisionModule()
{
	if ( nikRastKarde )
	{
		nikRastKarde->close();
	}
}

void VisionModule::ProcessVision ( WorldState * state )
{
	if ( !connected )
	{
		cout << "	Hey you! Put the LAN cable back in its socket, or ..." << endl;
		return;
		//connectToVisionServer ( setting -> UDP_Adress , setting -> LocalPort );
	}

	while ( ! ( ( packet_recieved[0] || ( ! setting -> use_camera[0] ) ) && ( packet_recieved[1] || ( ! setting -> use_camera[1] ) ) ) )
		recievePacket ( );

	ProcessBalls ( state );
	ProcessRobots ( state );

	packet_recieved[0] = 0;
	packet_recieved[1] = 0;

}

VisionSetting * VisionModule::GetSetting ( void )
{
	return setting;
}