#include "Vision/Vision.h"
#include "Referee/referee_new.h"
#include "Referee/referee.h"
#include <GameSetting.h>
#include <timer.h>
#include <kbhit.h>
#include <thread>
#include <mutex>
#include <protos/messages_immortals_wrapper.pb.h>
#include <math/Random.h>
#include <com/proto_bridge.h>
#include <protos/messages_robocup_ssl_wrapper.pb.h>
#include <protos/messages_immortals_configs.pb.h>

int main()
{
	GameSetting * setting = new GameSetting();

	setting->visionSetting = new VisionSetting();
	setting->visionSetting->color = COLOR_BLUE;
	setting->visionSetting->UDP_Adress = "224.5.23.2";
	setting->visionSetting->LocalPort = 10006;
	setting->visionSetting->ZmqPort = 5556;
	setting->visionSetting->use_camera.push_back(true);
	setting->visionSetting->use_camera.push_back(false);
	setting->visionSetting->use_camera.push_back(false);
	setting->visionSetting->use_camera.push_back(false);

	setting->side = Right;

	auto config = new Immortals::Data::GameConfig();

	config->set_side(Immortals::Data::FieldSide::Right);

	auto visionConfig = config->mutable_vision_config();
	visionConfig->set_our_color(Immortals::Data::TeamColor::Blue);
	visionConfig->set_vision_address("224.5.23.2");
	visionConfig->set_vision_port(10006);
	visionConfig->set_zmq_world_pub("tcp://*:5556");
	visionConfig->set_zmq_cmd_sub("tcp://localhost:5557");
	visionConfig->set_zmq_feedback_sub("tcp://localhost:5558");
	visionConfig->add_camera_enabled(true);
	visionConfig->add_camera_enabled(true);
	visionConfig->add_camera_enabled(false);
	visionConfig->add_camera_enabled(false);
	visionConfig->set_camera_count(4);
	visionConfig->set_predict_steps(5.0f);
	visionConfig->set_max_balls(10);
	visionConfig->set_max_ball_not_seen(40);
	visionConfig->set_max_robots(12);
	visionConfig->set_max_robot_not_seen(600);
	visionConfig->set_max_robot_subsitute(60);
	visionConfig->set_merge_distance(70.0f);
	visionConfig->set_ball_buffer_frames(30);
	visionConfig->set_ignore_prediction(0.045f);
	visionConfig->set_ball_error_velocity(1960000.0f);
	visionConfig->set_robot_error_velocity(200000.0f);
	visionConfig->set_max_ball_2_frames_dis(671.0f);

	auto com_config = config->mutable_com_config();
	com_config->set_sender_address("224.5.23.3");
	com_config->set_sender_port(60006);
	com_config->set_zmq_cmd_sub("tcp://localhost:5557");
	com_config->set_zmq_feedback_pub("tcp://*:5558");


	WorldState state;

	Referee referee;
	NewReferee newReferee;

	referee.Init("224.5.23.1", 10001, setting->visionSetting->color);
	cout << " Connecting to RefereeBox server at " << "224.5.23.1" << " , on port : 10001 " << endl;
	if (!referee.Open())
	{
		cout << "	Hey you! Put the LAN cable back in its socket, or ..." << endl;
	}

	/*newReferee.init ( "224.5.23.1" , 64003 , setting -> visionSetting -> color );
	cout << " Connecting to NEW RefereeBox server at " << "224.5.23.1" << " , on port : 10003 " << endl;
	if ( !newReferee.connect ( ) )
	{
	cout << "	Hey you! Put the LAN cable back in its socket, or ..." << endl;
	}*/

	cout << " Connecting to SSL-Vision server at " << "224.5.23.2" << " , on port : 10002 " << endl;
	Vision vision(config->vision_config());
	if (!vision.IsConnected())
	{
		cout << "	Hey you! Put the LAN cable back in its socket, or ..." << endl;
		return 0;
	}

	Timer timer;

	cout << " Now it is time, lets rock..." << endl;

	mutex lock;
	bool exited = false;

	auto vision_func = [&]()
	{
		while (true)
		{
			timer.start();

			vision.Process(state);

			lock.lock();
			vision.Publish(state);
			lock.unlock();

			cout << 1.0 / timer.interval() << endl;
		}
		exited = true;
	};
	auto ref_func = [&]()
	{
		while (!exited)
		{
			if (referee.Recieve())
			{
				//cout << "Referre Boz" << endl;
				lock.lock();
				referee.Process(state);
				lock.unlock();
				//cout << "Referre Boz" << endl;
			}
		}
	};
	auto new_ref_func = [&]()
	{
		while (!exited && !kbhit())
		{
			if (newReferee.Recieve())
			{
				//cout << "Referre Boz" << endl;
				lock.lock();
				newReferee.Process(state);
				//cout << "OPP GK IS: " << newReferee.oppGK << endl;
				lock.unlock();
				//cout << "Referre Boz" << endl;
			}
		}
	};

	auto com_rcv_func = [&]()
	{
		Net::UDP udp;
		if (!udp.open(60006, false, false, true))
		{
			fprintf(stderr, "Unable to open UDP network port: %d\n", 60006);
			fflush(stderr);
			return(false);
		}

		Net::Address multiaddr, interf;
		multiaddr.setHost("224.5.23.3", 60006);

		interf.setAny();

		if (!udp.addMulticast(multiaddr, interf)) {
			fprintf(stderr, "Unable to setup UDP multicast\n");
			fflush(stderr);
			return(false);
		}

		void *const zmq_context = zmq_ctx_new();
		void *const zmq_publisher = zmq_socket(zmq_context, ZMQ_PUB);

		int rc = zmq_bind(zmq_publisher, config->com_config().zmq_feedback_pub().c_str());
		assert(rc == 0);

		const int buffer_size = 65536;
		uint8_t* buffer = new uint8_t[buffer_size];
		Immortals::Data::RobotMessage message;

		while (true)
		{
			Net::Address src;
			int r = 0;
			r = udp.recv(buffer, 65536, src);
			if (r > 0) {
				
				bool result = feedback_bytes_to_proto_feedback(buffer, r, &message);
				if (!result)
					continue;

				const int length = message.ByteSize();
				assert(length < buffer_size);

				message.SerializeToArray(buffer, length);

				zmq_send(zmq_publisher, buffer, length, 0);
			}
		}

		zmq_close(zmq_publisher);
		zmq_ctx_destroy(zmq_context);
		delete[] buffer;
	};

	auto com_send_func = [&]()
	{
		Net::UDP udp;
		udp.open();

		Net::Address dest_address;
		dest_address.setHost(config->com_config().sender_address().c_str(), config->com_config().sender_port());

		void *const zmq_context = zmq_ctx_new();
		void *const zmq_subscriber = zmq_socket(zmq_context, ZMQ_SUB);
		int rc = zmq_connect(zmq_subscriber, config->com_config().zmq_cmd_sub().c_str());
		assert(rc == 0);

		const char *filter = "";
		rc = zmq_setsockopt(zmq_subscriber, ZMQ_SUBSCRIBE,
			filter, strlen(filter));
		assert(rc == 0);

		const int buffer_size = 65536;
		uint8_t* buffer = new uint8_t[buffer_size];

		Immortals::Data::RobotMessageFrame message_frame;

		while (true)
		{
			int received_size = zmq_recv(zmq_subscriber, buffer, buffer_size, 0);

			printf("Received cmd of size %d\n", received_size);

			if (!message_frame.ParseFromArray(buffer, received_size))
				continue;

			uint8_t payload[20 * (MAX_PAYLOAD_SIZE + 1)];

			const size_t size = proto_msg_frame_to_byte_array(message_frame, payload);

			memset(payload + size, 0, MAX_PAYLOAD_SIZE + 1);
			payload[size] = 25;
			payload[size + 1] = 110;
			payload[size + 2] = 80;

			bool res = udp.send(payload, size + MAX_PAYLOAD_SIZE + 1, dest_address);

			printf("sendto (%lu): %d\n", size, res);

			Sleep(16);
		}

		zmq_close(zmq_subscriber);
		zmq_ctx_destroy(zmq_context);
		delete[] buffer;
	};

	auto vision_test_send = [&]()
	{
		Net::UDP mc;
		int _port = 10006;
		string _net_address = "224.5.23.2";
		string _net_interface = "";
		if (!mc.open(_port, true, true)) {
			fprintf(stderr, "Unable to open UDP network port: %d\n", _port);
			fflush(stderr);
			return;
		}

		Net::Address multiaddr, _interface;
		multiaddr.setHost(_net_address.c_str(), _port);
		if (_net_interface.length() > 0) {
			_interface.setHost(_net_interface.c_str(), _port);
		}
		else {
			_interface.setAny();
		}

		if (!mc.addMulticast(multiaddr, _interface)) {
			fprintf(stderr, "Unable to setup UDP multicast\n");
			fflush(stderr);
			return;
		}
		SSL_WrapperPacket packet;
		packet.mutable_detection()->set_camera_id(0);
		
		auto ball = packet.mutable_detection()->add_balls();
		ball->set_confidence(1.f);
		ball->set_x(500);
		ball->set_y(500);
		ball->set_pixel_x(5);
		ball->set_pixel_y(5);

		Random random(-10, 10);

		while(!exited)
		{
			this_thread::sleep_for(chrono::milliseconds(16));

			auto now = std::chrono::system_clock::now();
			auto timestamp = now.time_since_epoch().count();

			packet.mutable_detection()->set_t_sent(timestamp);
			packet.mutable_detection()->set_t_capture(timestamp);
			packet.mutable_detection()->set_frame_number(packet.detection().frame_number() + 1);

			
			ball = packet.mutable_detection()->mutable_balls(0);
			ball->set_confidence(1.f);
			ball->set_x(500 + random.get());
			ball->set_y(500 + random.get());
			ball->set_pixel_x(5);
			ball->set_pixel_y(5);

			string buffer;
			packet.SerializeToString(&buffer);
			Net::Address multiaddr;
			multiaddr.setHost(_net_address.c_str(), _port);
			bool result;
			result = mc.send(buffer.c_str(), buffer.length(), multiaddr);
			if (result == false) {
				perror("Sendto Error");
				fprintf(stderr,
					"Sending UDP datagram to %s:%d failed (maybe too large?). "
					"Size was: %lu byte(s)\n",
					_net_address.c_str(),
					_port,
					buffer.length());
			}
		}

	};

	thread ai_thread(vision_func);
	thread ref_thread(ref_func);
	//thread new_ref_thread(new_ref_func);
	thread vision_test_send_thread(vision_test_send);

	ai_thread.join();
	ref_thread.join();
	//new_ref_thread.join();
	vision_test_send_thread.join();

	delete setting;
	
	return 0;
}
