#include "Vision/Vision.h"
#include "Referee/referee_new.h"
#include "Referee/referee.h"
#include <GameSetting.h>
#include <timer.h>
#include <kbhit.h>
#include <thread>
#include <mutex>
#include <protos/messages_robocup_ssl_wrapper.pb.h>

int main()
{
	GameSetting * setting = new GameSetting();

	setting->visionSetting = new VisionSetting();
	setting->visionSetting->color = COLOR_BLUE;
	setting->visionSetting->UDP_Adress = "224.5.23.2";
	setting->visionSetting->LocalPort = 10006;
	setting->visionSetting->GUI_Adress = "224.5.66.6";
	setting->visionSetting->GUIPort = 10009;
	setting->visionSetting->use_camera.push_back(true);
	setting->visionSetting->use_camera.push_back(false);
	setting->visionSetting->use_camera.push_back(false);
	setting->visionSetting->use_camera.push_back(false);

	setting->side = Right;

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
	Vision vision(setting->visionSetting);
	if (!vision.IsConnected())
	{
		cout << "	Hey you! Put the LAN cable back in its socket, or ..." << endl;
		return 0;
	}

	Timer timer;

	cout << " Now it is time, lets rock..." << endl;

	mutex lock;
	bool exited = false;

	auto ai_func = [&]()
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

		while(!exited)
		{
			this_thread::sleep_for(16ms);

			auto now = std::chrono::system_clock::now();
			auto timestamp = now.time_since_epoch().count();

			packet.mutable_detection()->set_t_sent(timestamp);
			packet.mutable_detection()->set_t_capture(timestamp);
			packet.mutable_detection()->set_frame_number(packet.detection().frame_number() + 1);

			


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
					"Size was: %zu byte(s)\n",
					_net_address.c_str(),
					_port,
					buffer.length());
			}
		}

	};

	thread ai_thread(ai_func);
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
