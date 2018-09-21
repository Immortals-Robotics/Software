#include "Vision.h"
#include <protos/messages_robocup_ssl_wrapper.pb.h>

#include <protos/messages_immortals_world_state.pb.h>

bool Vision::Open ( const std::string & address , const unsigned short port )
{
	VisionUDP = std::make_unique<Net::UDP>();
	if (!VisionUDP->open(port, true, true, true))
	{
		connected = false;
		return false;
	}
	Net::Address multiaddr, interf;
	multiaddr.setHost(address.c_str(), port);
	interf.setAny();

	if (!VisionUDP->addMulticast(multiaddr, interf)) {
		connected = false;
		return false;
	}

	connected = true;
	return true;
}

bool Vision::Receive(void)
{
	if (!connected)
		return false;
	const size_t MAX_INCOMING_PACKET_SIZE = 1000;
	char incoming_buffer[MAX_INCOMING_PACKET_SIZE];
	Net::Address src;
	int incoming_size = VisionUDP->recv(incoming_buffer, MAX_INCOMING_PACKET_SIZE, src);

	if (incoming_size <= 0)
	{
		cerr << "received a packet of size 0" << endl;
		return false;
	}

	SSL_WrapperPacket packet;
	packet.ParseFromArray(incoming_buffer, incoming_size);

	if (!packet.has_detection())
		return false;

	if (packet.detection().has_file_id())
	{
		file_id = packet.detection().file_id();
		printf("file id: %u\n", file_id);
	}

	frame[packet.detection().camera_id()] = packet.detection();
	packet_received[packet.detection().camera_id()] = true;

	return true;
}

bool Vision::IsConnected ( void ) const
{
	return connected;
}
void Vision::Publish(const WorldState& state) const
{
	static Immortals::Data::WorldState worldState;

	worldState.clear_id();
	worldState.clear_timestamp();
	worldState.clear_ball();
	worldState.clear_balls();
	worldState.clear_own_robots();
	worldState.clear_opp_robots();
	worldState.clear_referee();

	//worldState.set_id(frameId);
	worldState.set_id(file_id);
	worldState.set_timestamp(0);

	if (state.has_ball)
	{
		auto ball = worldState.mutable_ball();
		ball->set_id(0);

		auto position = ball->mutable_position();
		position->set_x(state.ball.Position.X);
		position->set_y(state.ball.Position.Y);
		position->set_z(0);

		auto velocity = ball->mutable_velocity();
		velocity->set_x(state.ball.velocity.x);
		velocity->set_y(state.ball.velocity.y);
		velocity->set_z(0);
	}

	// TODO: change it when multi-ball tracking is implemented
	//worldState.add_balls()->CopyFrom(worldState.ball());


	for (int team = 0; team < 2; team++)
	{
		for (int i = 0; i < config.max_robots(); i++)
		{
			if (robotState[team][i].seenState == CompletelyOut)
			{
				continue;
			}

			auto robot = (team == 0) ?
						 worldState.add_own_robots() :
						 worldState.add_opp_robots();

			robot->set_id(robotState[team][i].vision_id);
			// TODO: FIX THIS :D
			robot->set_color(config.our_color());
			auto position = robot->mutable_position();
			position->set_x(robotState[team][i].Position.X);
			position->set_y(robotState[team][i].Position.Y);
			auto velocity = robot->mutable_velocity();
			velocity->set_x(robotState[team][i].velocity.x);
			velocity->set_y(robotState[team][i].velocity.y);
			robot->set_orientation(robotState[team][i].Angle);
			robot->set_omega(robotState[team][i].AngularVelocity);
			robot->set_out_for_subsitute(robotState[team][i].OutForSubsitute);
		}
	}
	
	auto buffer = worldState.SerializeAsString();

	zmq_send(zmq_publisher, buffer.c_str(), buffer.size(), 0);

}
