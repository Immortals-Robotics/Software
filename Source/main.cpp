#include <iostream>
#include <fstream>
#include <thread>
#include <mutex>
#include <string.h>
#include <zconf.h>

#include "Common/GameSetting.h"
#include "Reality/Vision/Vision.h"
#include "Reality/Referee_2018/NewReferee.h"
#include "Soccer/ai09/ai09.h"
#include "Common/kbhit.h"
#include "Common/timer.h"
#include "Common/net_log.h"
#include "Common/Vector.h"
#include "Network/PracticalSocket.h"
#include "Soccer/aiBase.h"

#include "Reality/Sender/Protocol/writer.h"

using namespace std;

#define ImmortalsIsTheBest true

int main ( )
{
	if (!ImmortalsIsTheBest) {
		cout << "ERROR: Immortals is not the best SSL team anymore." << endl;
		cout << "Shutting down the system..." << endl;
        union FLOAT_32 minus,plus;
        minus.f32 = +20.0;
        plus.f32 = +20.0;
        uint8_t databuf[2];
        convert_float_to_2x_buff(databuf,22.0);
        cout<<(int) databuf[0] << ',' <<(int) databuf[1]<<endl;
        convert_float_to_2x_buff(databuf,-22.0);
        cout<<(int) databuf[0] << ',' <<(int) databuf[1]<<endl;

		while(!kbhit());
		return 0;
	}

    WorldState * state = new WorldState();
    
    
	GameSetting * settings = new GameSetting();
    settings -> use_camera.push_back(true);
    settings -> use_camera.push_back(true);
    settings -> use_camera.push_back(false);
    settings -> use_camera.push_back(false);
    
    settings -> use_camera.push_back(false);
    settings -> use_camera.push_back(false);
    settings -> use_camera.push_back(false);
    settings -> use_camera.push_back(false);

	settings -> our_color = COLOR_YELLOW;
    settings -> our_side = RIGHT_SIDE;
    settings -> referee_UDP_Address = "224.5.23.1";//TODO Default is "224.5.23.1"
    settings -> refereePort = 10003;
    settings -> vision_UDP_Address = "224.5.23.2";
    settings -> visionPort = 10006;


    cout << " Connecting to RefereeBox server at " << settings->referee_UDP_Address
         << " , on port : " << settings->refereePort << endl;
    NewReferee referee_2018(settings,state);
    if ( referee_2018.connectToRefBox () ){
        cout<<"Connected to RefBox successfully :)"<<endl;
    }else{
        cout << "	Hey you! Put the LAN cable back in its socket, or ..." << endl;
    }

    cout << " Connecting to Vision server at " << settings->vision_UDP_Address
         << " , on port : " << settings->visionPort << endl;
    VisionModule vision ( settings, state );
    if ( vision.isConnected() ){
        cout<<"Connected to Vision successfully :)"<<endl;
    }else{
        cout << "	Hey you! Put the LAN cable back in its socket, or ..." << endl;
        //return 0;
    }

    Sender* senderBase = new Sender;

	UDPSocket commUDP;
	char robot_cmds[90];
	char zeros[90];

	for ( int i = 0 ; i < 90 ; i ++ )
	{
		zeros[i] = 0;
		robot_cmds[i]=0;
	}


//	robot_cmds[66] = 25;
//	robot_cmds[77] = 80;
//	robot_cmds[78] = rf_freq;
//	robot_cmds[84] = rf_freq;

	bool started = false;

	aiBase * aii = new ai09(state,settings,senderBase);

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
        while ( (! kbhit()) && ( ImmortalsIsTheBest ) )	//Hope it lasts Forever...
        {
            timer.start();
            lock.lock();

            //The vision process
            vision.ProcessVision();
            //The AI process
            aii -> Process( state , settings , robot_cmds );
            //The sending process
            senderBase->sendAll();

            lock.unlock();
            cout << 1.0/timer.interval() << endl;
            started = true;
        }
        exited = true;
        commUDP.sendTo ( zeros , 10 , "localhost" , 60001 );
        commUDP.sendTo ( zeros , 1 , "localhost" , 60006 );
    };

    auto ref_func = [&]()
    {
        while ( ( !exited ) && (! kbhit()) && ( ImmortalsIsTheBest ) )	//Hope it lasts Forever...
        {
            if ( referee_2018.recieve() )
            {
                //cout << "Referre Boz" << endl;
                lock.lock();
                referee_2018.process();
                lock.unlock();
//                cout << "Referre Boz "<< referee_2018.command_CNT << endl;

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
        UDPSocket* strategyUDP = new UDPSocket(60006);
        strategyUDP -> joinGroup("224.5.23.3");
        const int strategyBufferMaxSize = 100000;
        char strategyBuffer[strategyBufferMaxSize];
        while ( ( !exited ) && (! kbhit()) && ( ImmortalsIsTheBest ) )	//Hope it lasts Forever...
        {
            string strategySrcAdd;
            unsigned short strategySrcPort;
            int strategySize = strategyUDP->recvFrom(strategyBuffer, strategyBufferMaxSize, strategySrcAdd, strategySrcPort);
            if ( strategySize > 11 )
            {
                cout << "Recieved \"strategy.ims\" with size: " << float(strategySize)/1000.0f << " KB, from " << strategySrcAdd << " on port " << strategySrcPort << "." << endl;
                lock.lock();
                dynamic_cast<ai09*>(aii)->read_playBook_str(strategyBuffer, strategySize);
                lock.unlock();
                ofstream strategyFile ( "../../strategy.ims" , ios::out|ios::binary);
                strategyFile.write(strategyBuffer, strategySize);
                strategyFile.close();
            }
            else if ( strategySize == 11 )
            {
                cout << "Recieved PID configs with size: " << float(strategySize) << " B, from " << strategySrcAdd << " on port " << strategySrcPort << "." << endl;
                strategyBuffer[0] = pid_det_index;//robot id
                pid_det_index = (pid_det_index+1)%MAX_ROBOTS;
                strategyBuffer[1] = 2;

                for (int i = 0; i < 11; i ++) {
                    //cout << (int)((unsigned char)strategyBuffer[i]) << endl;
                }
                commUDP.sendTo ( strategyBuffer , 11 , "224.5.92.5" , 60005 );

                /*for ( int i = 0 ; i < 20 ; i ++ )
                {
                strategyBuffer[1] = 1;
                strategyBuffer[2] = 60*(min(((float)i)/3.0, 1.0));
                strategyBuffer[3] = 60*(min(((float)i)/3.0, 1.0));
                strategyBuffer[4] = 0;
                strategyBuffer[5] = 0;
                strategyBuffer[6] = 0;
                strategyBuffer[7] = 0;
                strategyBuffer[8] = 0;
                strategyBuffer[9] = 0;
                strategyBuffer[10] = 1;
                commUDP.sendTo ( strategyBuffer , 11 , "224.5.92.5" , 60005 );
                    timer.start();
                while (timer.time()*1000.0f<160.6f);
                }
                for ( int i = 0 ; i < 18 ; i ++ )
                {
                    strategyBuffer[1] = 1;
                    strategyBuffer[2] = 60*(min(((float)i)/3.0, 1.0));
                    strategyBuffer[3] = 60*(min(((float)i)/3.0, 1.0));
                    strategyBuffer[4] = 0;
                    strategyBuffer[5] = 0;
                    strategyBuffer[6] = 0;
                    strategyBuffer[7] = 0;
                    strategyBuffer[8] = 0;
                    strategyBuffer[9] = 0;
                    strategyBuffer[10] = 49;
                    commUDP.sendTo ( strategyBuffer , 11 , "224.5.92.5" , 60005 );
                    timer.start();
                    while (timer.time()*1000.0f<160.6f);
                }*/
            }

            else if ( strategySize == 10 )
            {
                //cout << "Recieved robot feedback with size: " << float(strategySize) << " B, from " << strategySrcAdd << " on port " << strategySrcPort << "." << endl;
                /*for (int i = 0 ; i < 10 ; i ++) {
                    cout << (int)strategyBuffer[i] << "	";
                }*/

                unsigned int gyroD[2];
                gyroD[0] = strategyBuffer[0];
                gyroD[1] = strategyBuffer[7];

                int tmpGyro = (gyroD[0]<<8) | gyroD[1];
                if(tmpGyro&0x8000)
                    tmpGyro = -((~tmpGyro+1)&0xFFFF);
                gyrOff = (gyrOff*offCount)+ (tmpGyro/14.375);
                offCount ++;
                gyrOff /= (float)(offCount);

                cout << gyrOff << "	" << tmpGyro/14.375;

                cout << endl;
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
	thread dbg_dump_thread(test_dbg_dump);

    ai_thread.join();
    ref_thread.join();
    //new_ref_thread.join();
    //sharifcup_thread.join();
    str_thread.join();
	dbg_dump_thread.join();

	delete settings;
	delete state;
	delete aii;

	return 0;
}
