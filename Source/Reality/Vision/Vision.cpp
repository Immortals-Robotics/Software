#include "Vision.h"

#include "Kalman/FilteredObject.h"

#include <fstream>
#include <assert.h>
using namespace std;

Vision::Vision ( VisionSetting * _setting ) : connected ( false ) 
{
	if ( _setting )
	{
		setting = _setting;

        while ( setting->use_camera.size() < CAM_COUNT )
            setting->use_camera.push_back(false);
        
		Open ( setting -> UDP_Adress , setting -> LocalPort );
	}

	frameId = 0;

	zmq_context = zmq_ctx_new ();
	zmq_publisher = zmq_socket (zmq_context, ZMQ_PUB);
	int rc = zmq_bind (zmq_publisher, "tcp://*:5556");
	assert (rc == 0);

    for ( int i = 0 ; i < CAM_COUNT ; i ++ )
        packet_recieved[i] = false;

	lastRawBall.set_x ( 0.0f );
	lastRawBall.set_y ( 0.0f );

	string fast_filter_path(DATA_PATH); fast_filter_path.append("/ballFilterFast.txt");
	string slow_filter_path(DATA_PATH); slow_filter_path.append("/ballFilterSlow.txt");

	ball_kalman.initialize(fast_filter_path.c_str(), slow_filter_path.c_str());
	
	for ( int i = 0 ; i < MAX_ROBOTS; i++ )
	{
		robot_kalman[0][i].initialize (fast_filter_path.c_str(), slow_filter_path.c_str());
		robot_kalman[1][i].initialize (fast_filter_path.c_str(), slow_filter_path.c_str());
		rawAngles[0][i] = 0.0f;
		rawAngles[1][i] = 0.0f;
	}
	
	ball_not_seen = MAX_BALL_NOT_SEEN + 1;
	for ( int i = 0 ; i < MAX_ROBOTS ; i ++ )
	{
		robot_not_seen[0][i] = MAX_ROBOT_NOT_SEEN + 1;
		robot_not_seen[1][i] = MAX_ROBOT_NOT_SEEN + 1;
	}
}
Vision::~Vision()
{
	zmq_close (zmq_publisher);
	zmq_ctx_destroy (zmq_context);
}

void Vision::Process ( WorldState& state )
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
        Receive();
        
    }
	
	ProcessBalls ( state );
	ProcessRobots ( state );

	for ( int i = 0 ; i < CAM_COUNT ; i ++ )
        packet_recieved[i] = false;

	frameId++;

}

const VisionSetting& Vision::GetSetting ( void ) const
{
	return *setting;
}
