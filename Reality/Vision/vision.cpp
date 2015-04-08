#include "vision.h"

#include "FilteredObject.h"

#include <fstream>
using namespace std;

VisionModule::VisionModule ( VisionSetting * _setting ) : connected ( false ) 
{
	if ( _setting )
	{
		setting = _setting;

        while ( setting->use_camera.size() < CAM_COUNT )
            setting->use_camera.push_back(false);
        
		connectToVisionServer ( setting -> UDP_Adress , setting -> LocalPort );
	}

	GUIUDP = new UDPSocket ( );

    for ( int i = 0 ; i < CAM_COUNT ; i ++ )
        packet_recieved[i] = false;

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
}

void VisionModule::ProcessVision ( WorldState * state )
{
	if ( !connected )
	{
		cout << "	Hey you! Put the LAN cable back in its socket, or ..." << endl;
		return;
		//connectToVisionServer ( setting -> UDP_Adress , setting -> LocalPort );
	}

    bool cams_ready = false;
    while (!cams_ready)
    {
        cams_ready = true;
        for ( int i = 0 ; i < CAM_COUNT ; i ++ )
        {
            bool new_cam_ready = packet_recieved[i] || (!setting -> use_camera[i]);
            if ( !new_cam_ready )
            {
                cams_ready = false;
                break;
            }
        }
        if ( cams_ready )
            break;
        //cout << "bodo dg    " << cams_ready << endl;
        recievePacket();
        
    }
	
	ProcessBalls ( state );
	ProcessRobots ( state );

	for ( int i = 0 ; i < CAM_COUNT ; i ++ )
        packet_recieved[i] = false;

}

VisionSetting * VisionModule::GetSetting ( void )
{
	return setting;
}