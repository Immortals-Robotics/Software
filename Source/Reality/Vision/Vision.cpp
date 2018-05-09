#include "Vision.h"

#include "Kalman/FilteredObject.h"
#include "../../Common/GameSetting.h"

#include <fstream>
#include <zconf.h>

using namespace std;

VisionModule::VisionModule(GameSetting* _settings,WorldState* _State) : connected ( false )
{
    playState = _State;

    if(_settings == NULL){
        return;
    }
    //Initializing the settings:
    our_color = _settings->our_color;
    our_side = _settings->our_side;

    vision_UDP_Address = _settings->vision_UDP_Address;
    visionPort = _settings->visionPort;

    use_camera = _settings->use_camera;
    while(use_camera.size() < CAM_COUNT)
        use_camera.push_back(false);

    for ( int i = 0 ; i < CAM_COUNT ; i ++ )
        packet_recieved[i] = false;

    for ( int i = 0 ; i < BALL_BUFFER_FRAMES ; i ++ )
    {
        ball_pos_buff[i] = Vec2(0.0,0.0);
    }

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

    ball_kalman.initialize(fast_filter_path.c_str(), slow_filter_path.c_str());

    //Launching UDP Connections
    if(!connectToVisionServer()){
        cout<<"Failed to connect to Vision UDP"<<endl;
    }




    /*Old Code
    setting = _settings;

    while ( setting->use_camera.size() < CAM_COUNT )
        setting->use_camera.push_back(false);

    connectToVisionServer ( setting -> UDP_Adress , setting -> LocalPort );


	GUIUDP = new UDPSocket ( );

	gui_zmq_context = zmq_ctx_new ();
	gui_zmq_publisher = zmq_socket (gui_zmq_context, ZMQ_PUB);
	int rc = zmq_bind (gui_zmq_publisher, "tcp://*:5556");
	assert (rc == 0);

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
	
	//InitANN();
	
	ball_not_seen = MAX_BALL_NOT_SEEN + 1;
	for ( int i = 0 ; i < MAX_ROBOTS ; i ++ )
	{
		robot_not_seen[0][i] = MAX_ROBOT_NOT_SEEN + 1;
		robot_not_seen[1][i] = MAX_ROBOT_NOT_SEEN + 1;
	}
     */

}
VisionModule::~VisionModule()
{
	zmq_close (gui_zmq_publisher);
	zmq_ctx_destroy (gui_zmq_context);
}

void VisionModule::ProcessVision() {
	if (!connected) {
		cout << "	Hey you! Put the LAN cable back in its socket, or ..." << endl;
		return;
		//connectToVisionServer ( setting -> UDP_Adress , setting -> LocalPort );
	}

	bool cams_ready = false;
	while (!cams_ready) {
		cams_ready = true;
		for (int i = 0; i < CAM_COUNT; i++) {
			bool new_cam_ready = packet_recieved[i] || (!this->use_camera[i]);
			if (!new_cam_ready) {
				cams_ready = false;
				break;
			}
		}
		if (cams_ready)
			break;
		//cout << "bodo dg    " << cams_ready << endl;
		recievePacket();

	}

	ProcessBalls ( this->playState );
	ProcessRobots ( this->playState );

	for ( int i = 0 ; i < CAM_COUNT ; i ++ )
        packet_recieved[i] = false;

}

