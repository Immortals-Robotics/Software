#include "Vision.h"

#include "Kalman/FilteredObject.h"

#include <fstream>
#include <assert.h>
using namespace std;

Vision::Vision (const Immortals::Data::VisionConfig& _config )
		: connected ( false ),
		  config(_config) {

	for (int i = 0; i < 2; i ++)
	{
		robot_kalman[i] = new FilteredObject[config.max_robots()];
		AngleFilter[i] = new MedianFilter<float>[config.max_robots()];
		rawAngles[i] = new float[config.max_robots()];
		robot_not_seen[i] = new int[config.max_robots()];
		robotState[i] = new RobotState[config.max_robots()];
	}
	frame = new SSL_DetectionFrame[config.camera_count()];
	d_ball = new SSL_DetectionBall[config.camera_count() * config.max_balls()];
	robot = new SSL_DetectionRobot[config.camera_count() * config.max_robots()];
	packet_received = new bool[config.camera_count()];

	Open(config.vision_address(), config.vision_port());

	frameId = 0;

	zmq_context = zmq_ctx_new();
	zmq_publisher = zmq_socket(zmq_context, ZMQ_PUB);

	//"tcp://*:5556"
	int rc = zmq_bind(zmq_publisher, config.zmq_world_pub().c_str());
	assert (rc == 0);

	for (int i = 0; i < config.camera_count(); i++)
		packet_received[i] = false;

	lastRawBall.set_x(0.0f);
	lastRawBall.set_y(0.0f);

	string fast_filter_path(DATA_PATH);
	fast_filter_path.append("/ballFilterFast.txt");
	string slow_filter_path(DATA_PATH);
	slow_filter_path.append("/ballFilterSlow.txt");

	ball_kalman.initialize(fast_filter_path.c_str(), slow_filter_path.c_str());

	for (int i = 0; i < config.max_robots(); i++) {
		robot_kalman[0][i].initialize(fast_filter_path.c_str(), slow_filter_path.c_str());
		robot_kalman[1][i].initialize(fast_filter_path.c_str(), slow_filter_path.c_str());
		rawAngles[0][i] = 0.0f;
		rawAngles[1][i] = 0.0f;
	}

	ball_not_seen = config.max_ball_not_seen() + 1;
	for (int i = 0; i < config.max_robots(); i++) {
		robot_not_seen[0][i] = config.max_robot_not_seen() + 1;
		robot_not_seen[1][i] = config.max_robot_not_seen() + 1;
	}
}

Vision::~Vision()
{
	for (int i = 0; i < 2; i ++)
	{
		delete robot_kalman[i];
		delete AngleFilter[i];
		delete rawAngles[i];
		delete robot_not_seen[i];
		delete robotState[i];
	}
	delete frame;
	delete d_ball;
	delete robot;
	delete packet_received;

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
        for ( int i = 0 ; i < config.camera_count(); i ++ )
        {
            bool new_cam_ready = packet_received[i] || (!config.camera_enabled(i));
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

	for ( int i = 0 ; i < config.camera_count(); i ++ )
        packet_received[i] = false;

	frameId++;

}

const Immortals::Data::VisionConfig& Vision::GetSetting ( void ) const
{
	return config;
}
