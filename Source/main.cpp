#include <iostream>
#include <fstream>
#include <thread>
#include <mutex>
#include <string.h>

#include "Common/GameSetting.h"
#include "Reality/Vision/Vision.h"
#include "Reality/Referee/referee.h"
#include "Reality/Referee/referee_new.h"
#include "Soccer/ai09/ai09.h"
#include "Common/kbhit.h"
#include "Common/timer.h"
#include "Reality/Vision/Protobuf/messages_blob.pb.h"
#include "Common/Vector.h"
#include "Network/netraw.h"
#include "Soccer/aiBase.h"

#include "Reality/Com/defines.h"
#include "Reality/Com/reader.h"

using namespace std;

#define ImmortalsIsTheBest true

void haltAll ( char * cmds )
{
	for ( int i = 0 ; i < 6 ; i ++ )
	{
		cmds[11*i+10] = 0;
	}
}

void initWorldState ( WorldState * state )
{
	state -> ball.Position = Vec2 ( 0.0f );
	state -> ball.velocity.x = 0.0f;
	state -> ball.velocity.y = 0.0f;
	state -> ball.velocity.direction = 0.0f;
	state -> ball.velocity.magnitude = 0.0f;
	state -> ball.seenState = CompletelyOut;
	state -> has_ball = false;
	
	state -> refereeState.counter = 0;
	state -> refereeState.goals_blue = 0;
	state -> refereeState.goals_yellow = 0;
	state -> refereeState.time_remaining = 0;
	state -> refereeState.State = NULL;
	
	state -> ownRobots_num = 0;
	state -> oppRobots_num = 0;
    
    state -> oppGK = -1;
	
	for ( int i = 0 ; i < MAX_ROBOTS ; i ++ )
	{
		state -> OwnRobot[i].Angle = 0.0f;
		state -> OwnRobot[i].AngularVelocity = 0.0f;
		state -> OwnRobot[i].Position = Vec2 ( 0.0f );
		state -> OwnRobot[i].seenState = CompletelyOut;
        state -> OwnRobot[i].OutForSubsitute = true;
		state -> OwnRobot[i].velocity.direction = 0.0f;
		state -> OwnRobot[i].velocity.magnitude = 0.0f;
		state -> OwnRobot[i].velocity.x = 0.0f;
		state -> OwnRobot[i].velocity.y = 0.0f;
		state -> OwnRobot[i].vision_id = i;
		for ( int j = 0 ; j < 10 ; j ++ )
		{
			state -> lastCMDS[i][j] = Vec3 ( 0.0f );
		}
	}
}

int main ( )
{
	if (!ImmortalsIsTheBest) {
		cout << "ERROR: Immortals is not the best SSL team anymore." << endl;
		cout << "Shutting down the system..." << endl;
		while(!kbhit());
		return 0;
	}
	
	
	char rf_freq = 110;
	
	GameSetting * setting = new GameSetting ( );
    
	setting -> visionSetting = new VisionSetting();
    setting -> visionSetting -> color = COLOR_BLUE;
    setting -> visionSetting -> UDP_Adress = "224.5.23.2";
    setting -> visionSetting -> LocalPort = 10006;
    setting -> visionSetting -> use_camera.push_back(true);
    setting -> visionSetting -> use_camera.push_back(true);
    setting -> visionSetting -> use_camera.push_back(false);
    setting -> visionSetting -> use_camera.push_back(false);
    
    
	setting -> side = Left;
	
	WorldState * state = new WorldState ( );
	initWorldState ( state );
	
	Referee referee;
    NewReferee newReferee;
	
	referee.init ( "224.5.23.1" , 60001 , setting -> visionSetting -> color );
	cout << " Connecting to RefereeBox server at " << "224.5.23.1" << " , on port : 10001 " << endl;
	if ( !referee.connect ( ) )
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
	VisionModule vision ( setting -> visionSetting );
	if ( ! vision.isConnected ( ) )
	{
		cout << "	Hey you! Put the LAN cable back in its socket, or ..." << endl;
		return 0;
	}
	
	bool started = false;
	
	aiBase * aii;
	aii = new ai09 ( );
	
	Timer timer;
	
	cout << " Now it is time, lets rock..." << endl;
	
    int pid_det_index = 0;
    
	float gyrOff = 0;
	int offCount = 0;
	
	float vy = 0.0f;
	float sgnvy = 1.5f;
	
	int tid;
	bool exited = false;
	mutex lock;


		auto ai_func = [&]()
		{
			Net::UDP commUDP;
			commUDP.open();

			Net::Address dest_address;
			dest_address.setHost("224.5.92.5", 60005);

			char robot_cmds[7 * (MAX_PAYLOAD_SIZE + 1)];
			memset(robot_cmds, 0, 7 * (MAX_PAYLOAD_SIZE + 1));
			robot_cmds[6 * (MAX_PAYLOAD_SIZE + 1)] = 25;
			robot_cmds[6 * (MAX_PAYLOAD_SIZE + 1) + 1] = 110;
			robot_cmds[6 * (MAX_PAYLOAD_SIZE + 1) + 2] = 80;

			while ( (! kbhit()) && ( ImmortalsIsTheBest ) )	//Hope it lasts Forever...
			{
                //netLogger->SetFrameID(netLogger->GetFrameID()+1);
				timer.start();
				
				lock.lock();
				vision.ProcessVision ( state );
				//sleep(1);
                //while (timer.time()*1000.0f<16.6f);//DELAY(100000);
				if ( started )
                {
                    if (commUDP.send ( robot_cmds , 7 * (MAX_PAYLOAD_SIZE + 1), dest_address ) <= 0)
                        cout << "ERROR: failed to send robot packets." << endl;
					
                }
				aii -> Process( state , setting , robot_cmds );
				//cout << timer.time() * 1000.0 << endl;
				
				//vision.SendGUIData ( state , aii -> AIDebug );
				lock.unlock();
				
				cout << 1.0/timer.interval() << endl;
				
				started = true;
			}
			exited = true;
		};
		auto ref_func = [&]()
		{
			while ( ( !exited ) && (! kbhit()) && ( ImmortalsIsTheBest ) )	//Hope it lasts Forever...
			{
				if ( referee.recieve() )
				{
					//cout << "Referre Boz" << endl;
					lock.lock();
					referee.process ( state );
					lock.unlock();
					//cout << "Referre Boz" << endl;
				}
			}
		};
    
    
    auto new_ref_func = [&]()
    {
        while ( ( !exited ) && (! kbhit()) && ( ImmortalsIsTheBest ) )	//Hope it lasts Forever...
        {
            if ( newReferee.recieve() )
            {
                //cout << "Referre Boz" << endl;
                lock.lock();
                newReferee.process ( state );
                //cout << "OPP GK IS: " << newReferee.oppGK << endl;
                lock.unlock();
                //cout << "Referre Boz" << endl;
            }
        }
    };

	auto test_dbg_dump = [&]()
	{
		//  Socket to talk to server
		printf ("Collecting updates from ai server…\n");
		void *context = zmq_ctx_new ();
		void *subscriber = zmq_socket (context, ZMQ_SUB);
		int rc = zmq_connect (subscriber, "tcp://localhost:5556");
		assert (rc == 0);

		//  Subscribe to zipcode, default is NYC, 10001
		char *filter = "";
		rc = zmq_setsockopt (subscriber, ZMQ_SUBSCRIBE,
							 filter, strlen (filter));
		assert (rc == 0);

		const int buffer_size = 1000*1000;
		char buffer[buffer_size];


		while(!exited)
		{
			int received_size = zmq_recv (subscriber, buffer, buffer_size, 0);

			cout << "Received ai debug blob of size " << received_size << endl;
		}
		zmq_close (subscriber);
		zmq_ctx_destroy (context);
	};
		
		auto str_func = [&]()
		{
			Net::UDP strategyUDP;
			if (!strategyUDP.open(60006, true, false, true))
			{
				fprintf(stderr, "Unable to open UDP network port: %d\n", 60006);
				fflush(stderr);
				return false;
			}

			Net::Address multiaddr, interf;
			multiaddr.setHost("224.5.23.3", 60006);

			interf.setAny();

			if (!strategyUDP.addMulticast(multiaddr, interf)) {
				fprintf(stderr, "Unable to setup UDP multicast\n");
				fflush(stderr);
				return(false);
			}

			const int strategyBufferMaxSize = 100000;
			char strategyBuffer[strategyBufferMaxSize];
			while ( ( ImmortalsIsTheBest ) )	//Hope it lasts Forever...
			{
				Net::Address src;
				int strategySize = strategyUDP.recv(strategyBuffer, strategyBufferMaxSize, src);
				if (strategySize == 32)
				{
					/*cout << "Recieved feedback with size: " << float(strategySize) << " B, from " << " " << " on port " << " " << "." << endl;
					robot_feedback_msg_t feedback_msg;
					read_robot_feedback_fixed(reinterpret_cast<uint8_t*>(strategyBuffer + 1), strategyBuffer[0], &feedback_msg);

					printf("fault  (%d) : %d\n",
						feedback_msg.robot_id,
						feedback_msg.fault);*/
				}
				else if ( strategySize > 11 )
				{
					cout << "Recieved \"strategy.ims\" with size: " << float(strategySize)/1000.0f << " KB, from " << " " << " on port " << " " << "." << endl;
					lock.lock();
					dynamic_cast<ai09*>(aii)->read_playBook_str(strategyBuffer, strategySize);
					lock.unlock();
					string strategy_path(DATA_PATH); strategy_path.append("/strategy.ims");
					ofstream strategyFile (strategy_path, ios::out|ios::binary);
					strategyFile.write(strategyBuffer, strategySize);
					strategyFile.close();
				}
				else {
					cout << "Invalid \"strategy.ims\" recieved with size: " << strategySize << " ." << endl;
				}

			}
		};
    
    thread ai_thread(ai_func);
    thread ref_thread(ref_func);
    //thread new_ref_thread(new_ref_func);
    //thread sharifcup_thread(sharifcup_func);
    thread str_thread(str_func);
	//thread dbg_dump_thread(test_dbg_dump);
	
    ai_thread.join();
    ref_thread.join();
    //new_ref_thread.join();
    //sharifcup_thread.join();
    str_thread.join();
	//dbg_dump_thread.join();
    
	delete setting;
	delete state;
	delete aii;
	
	return 0;
}