#include "Vision.h"

#include "Kalman/FilteredObject.h"
#include <assert.h>

Vision::Vision (const Immortals::Data::VisionConfig& _config )
		: connected ( false ),
		  config(_config) {

	for (int i = 0; i < 2; i ++)
	{
		robot_kalman[i].resize(config.max_robots());
		AngleFilter[i].resize(config.max_robots());
		rawAngles[i].resize(config.max_robots());
		robot_not_seen[i].resize(config.max_robots());
		robotState[i].resize(config.max_robots());
	}

	ballState.resize(config.max_balls());

	frame.resize(config.camera_count());
	d_ball.resize(config.camera_count() * config.max_balls());
	robot.resize(config.camera_count() * config.max_robots());
	packet_received.resize(config.camera_count());

	Open(config.vision_address(), config.vision_port());

	frameId = 0;

	zmq_context = zmq_ctx_new();
	zmq_publisher = zmq_socket(zmq_context, ZMQ_PUB);

	//"tcp://*:5556"
	int rc = zmq_bind(zmq_publisher, config.zmq_world_pub().c_str());
	assert (rc == 0);

	for (int i = 0; i < config.camera_count(); i++)
	{
		packet_received[i] = false;
	}

	string fast_filter_path(DATA_PATH);
	fast_filter_path.append("/ballFilterFast.txt");
	string slow_filter_path(DATA_PATH);
	slow_filter_path.append("/ballFilterSlow.txt");

	ball_kalman.resize(config.max_balls());
	ball_not_seen.resize(config.max_balls());
	for (int i = 0; i < config.max_balls(); i++)
	{
		ball_not_seen[i] = config.max_ball_not_seen() + 1;
		ball_kalman[i].initialize(fast_filter_path.c_str(), slow_filter_path.c_str());
	}

	for (int i = 0; i < config.max_robots(); i++) 
	{
		robot_kalman[0][i].initialize(fast_filter_path.c_str(), slow_filter_path.c_str());
		robot_kalman[1][i].initialize(fast_filter_path.c_str(), slow_filter_path.c_str());
		rawAngles[0][i] = 0.0f;
		rawAngles[1][i] = 0.0f;
	}
	
	for (int i = 0; i < config.max_robots(); i++) 
	{
		robot_not_seen[0][i] = config.max_robot_not_seen() + 1;
		robot_not_seen[1][i] = config.max_robot_not_seen() + 1;
	}

	constexpr double constant_elapsed = 1000. / 30.;
	blue_id_generator = std::make_unique<IdGenerator>(constant_elapsed);
	yellow_id_generator = std::make_unique<IdGenerator>(constant_elapsed);
	ball_id_generator = std::make_unique<IdGenerator>(constant_elapsed);
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

	if (capture_id == 0)
	{
		ResetState(state);
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

void Vision::ResetState(WorldState& state)
{
	frameId = 0;
	
	for (int i = 0; i < config.camera_count(); i++)
	{
		packet_received[i] = false;
	}

	const float init_pos[2]{ 0.f, 0.f };

	for (int i = 0; i < config.max_balls(); i++)
	{
		ball_not_seen[i] = config.max_ball_not_seen() + 1;
		ball_kalman[i].initializePos(init_pos);
	}

	for (int i = 0; i < config.max_robots(); i++)
	{
		robot_kalman[0][i].initializePos(init_pos);
		robot_kalman[0][i].initializePos(init_pos);

		rawAngles[0][i] = 0.0f;
		rawAngles[1][i] = 0.0f;
	}

	for (int i = 0; i < config.max_robots(); i++)
	{
		robot_not_seen[0][i] = config.max_robot_not_seen() + 1;
		robot_not_seen[1][i] = config.max_robot_not_seen() + 1;
	}

	for (int i = 0; i < config.max_robots(); i++)
	{
		robotState[0][i].seenState = CompletelyOut;
		robotState[1][i].seenState = CompletelyOut;
	}

	blue_id_generator->Reset();
	yellow_id_generator->Reset();
	ball_id_generator->Reset();

	state.refereeState.counter = 0;
	state.refereeState.goals_blue = 0;
	state.refereeState.goals_yellow = 0;
	state.refereeState.time_remaining = 0;
	state.refereeState.State = NULL;

	state.balls_num = 0;
	state.ownRobots_num = 0;
	state.oppRobots_num = 0;

	state.oppGK = -1;

	state.has_ball = false;

	for (int i = 0; i < MAX_ROBOTS; i++)
	{
		state.balls[i].Position = Vec2(0.0f);
		state.balls[i].velocity.x = 0.0f;
		state.balls[i].velocity.y = 0.0f;
		state.balls[i].velocity.direction = 0.0f;
		state.balls[i].velocity.magnitude = 0.0f;
		state.balls[i].seenState = CompletelyOut;
	}

	for (int i = 0; i < MAX_ROBOTS; i++)
	{
		state.OwnRobot[i].Angle = 0.0f;
		state.OwnRobot[i].AngularVelocity = 0.0f;
		state.OwnRobot[i].Position = Vec2(0.0f);
		state.OwnRobot[i].seenState = CompletelyOut;
		state.OwnRobot[i].OutForSubsitute = true;
		state.OwnRobot[i].velocity.direction = 0.0f;
		state.OwnRobot[i].velocity.magnitude = 0.0f;
		state.OwnRobot[i].velocity.x = 0.0f;
		state.OwnRobot[i].velocity.y = 0.0f;
		state.OwnRobot[i].vision_id = i;
		for (int j = 0; j < 11; j++)
		{
			state.lastCMDS[i][j] = Vec3(0.0f);
		}
	}
}
