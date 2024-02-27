#include "Vision.h"

#include "Kalman/FilteredObject.h"
#include "../../Common/GameSetting.h"

#include <fstream>

VisionModule::VisionModule(GameSetting* _settings,WorldState* _State)
{
    playState = _State;

    if(_settings == NULL){
        return;
    }
    //Initializing the settings:
    our_color = _settings->our_color;
    our_side = _settings->our_side;

    for ( int i = 0 ; i < Setting::kCamCount ; i ++ )
        packet_recieved[i] = false;

    for ( int i = 0 ; i < BALL_BUFFER_FRAMES ; i ++ )
    {
        ball_pos_buff[i] = Vec2(0.0,0.0);
    }

    lastRawBall.set_x ( 0.0f );
    lastRawBall.set_y ( 0.0f );

    std::string fast_filter_path(DATA_DIR); fast_filter_path.append("/ballFilterFast.txt");
    std::string slow_filter_path(DATA_DIR); slow_filter_path.append("/ballFilterSlow.txt");

    ball_kalman.initialize(fast_filter_path.c_str(), slow_filter_path.c_str());

    for ( int i = 0 ; i < Setting::kMaxRobots; i++ )
    {
        robot_kalman[0][i].initialize (fast_filter_path.c_str(), slow_filter_path.c_str());
        robot_kalman[1][i].initialize (fast_filter_path.c_str(), slow_filter_path.c_str());
        rawAngles[0][i] = 0.0f;
        rawAngles[1][i] = 0.0f;
    }

    ball_kalman.initialize(fast_filter_path.c_str(), slow_filter_path.c_str());

    //Launching UDP Connections
    if(!connectToVisionServer()){
        std::cout<<"Failed to connect to Vision UDP"<< std::endl;
    }

}
VisionModule::~VisionModule()
{
}

void VisionModule::recieveAllCameras( void )
{
    if (!isConnected()) {
        std::cout << "	Hey you! Put the LAN cable back in its socket, or ..." << std::endl;
        return;
        //connectToVisionServer ( setting -> UDP_Adress , setting -> LocalPort );
    }

    bool cams_ready = false;
    while (!cams_ready) {
        cams_ready = true;
        for (int i = 0; i < Setting::kCamCount; i++) {
            bool new_cam_ready = packet_recieved[i] || (!setting().use_camera[i]);
            if (!new_cam_ready) {
                cams_ready = false;
                break;
            }
        }
        if (cams_ready)
            break;
        //std::cout << "bodo dg    " << cams_ready << std::endl;
        recievePacket();

    }
}

void VisionModule::ProcessVision()
{
	ProcessBalls ( this->playState );
	ProcessRobots ( this->playState );
    ProcessParam ( this->playState );


	for ( int i = 0 ; i < Setting::kCamCount ; i ++ )
        packet_recieved[i] = false;

}

