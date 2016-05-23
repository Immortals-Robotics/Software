#include "Vision.h"
#include <protos/messages_robocup_ssl_wrapper.pb.h>

#include <protos/messages_immortals_world_state.pb.h>

bool Vision::Open ( const std::string & address , const unsigned short port )
{
	VisionUDP = new Net::UDP();
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

	char incoming_buffer[MAX_INCOMING_PACKET_SIZE];
	Net::Address src;
	int incoming_size = VisionUDP->recv(incoming_buffer, MAX_INCOMING_PACKET_SIZE, src);

	if (incoming_size <= 0)
		return false;

	SSL_WrapperPacket packet;
	packet.ParseFromArray(incoming_buffer, incoming_size);

	if (!packet.has_detection())
		return false;
	frame[packet.detection().camera_id()] = packet.detection();
	packet_recieved[packet.detection().camera_id()] = true;

	return true;
}

bool Vision::IsConnected ( void ) const
{
	return connected;
}
void Vision::Publish(const WorldState& state) const
{
	Immortals::Data::WorldState _state;
	_state.set_id(frameId);
	_state.set_timestamp(0);

	if (state.has_ball)
	{
		auto ball = _state.mutable_ball();
		ball->set_id(0);

		ball->mutable_position()->set_x(state.ball.Position.X);
		ball->mutable_position()->set_y(state.ball.Position.Y);
		ball->mutable_position()->set_z(0);

		ball->mutable_velocity()->set_x(state.ball.velocity.x);
		ball->mutable_velocity()->set_y(state.ball.velocity.y);
		ball->mutable_velocity()->set_z(0);
	}

	
	auto buffer = _state.SerializeAsString();

	zmq_send(zmq_publisher, buffer.c_str(), buffer.size(), 0);

}
