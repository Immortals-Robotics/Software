#include <iostream>
#include <fstream>
#include <thread>
#include <mutex>
#include <string>

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
#include "Reality/Sender/Protocol/reader.h"

#include "Reality/Debugger/debuggerBase.h"
#include "grsim_fwd.h"

using namespace std;

#define ImmortalsIsTheBest true

int main ( )
{
    Services::initialize();

	if (!ImmortalsIsTheBest) {
		LOG_CRITICAL("Immortals is not the best SSL team anymore.");
		LOG_CRITICAL("Shutting down the system...");
	}

    WorldState * state = new WorldState();

	GameSetting * settings = new GameSetting();
    settings -> use_camera.push_back(true);// TODO #1 Check the cameras to be true
    settings -> use_camera.push_back(true);
    settings -> use_camera.push_back(true);
    settings -> use_camera.push_back(true);
    
    settings -> use_camera.push_back(true);
    settings -> use_camera.push_back(true);
    settings -> use_camera.push_back(true);
    settings -> use_camera.push_back(true);

	settings -> our_color = COLOR_YELLOW;
    settings -> our_side = LEFT_SIDE;

    const char *const xbox_ref_ip = "224.5.25.25";
    const char *const real_ref_ip = "224.5.23.1";
    settings -> referee_UDP_Address = real_ref_ip;// TODO #2 Check the referee input source
    settings -> refereePort = 10003;
    settings -> vision_UDP_Address = "224.5.23.2";
    settings -> visionPort = 10006;//10020;
    settings -> GUI_UDP_Address = "127.0.0.1";
    settings -> GUIPort = 10066;

    LOG_INFO(" Connecting to RefereeBox server at {} on port : {}", 
        settings->referee_UDP_Address, settings->refereePort);
    NewReferee referee_2018(settings,state);
    if ( referee_2018.connectToRefBox () ){
        LOG_INFO("Connected to RefBox successfully :)");
    }else{
        LOG_ERROR("Hey you! Put the LAN cable back in its socket, or ...");
    }

    LOG_INFO("Connecting to Vision server at {} on port: {}", 
        settings->vision_UDP_Address, settings->visionPort);
    VisionModule vision ( settings, state );
    if ( vision.isConnected() ){
        LOG_INFO("Connected to Vision successfully :)");
    }else{
        LOG_ERROR("Hey you! Put the LAN cable back in its socket, or ...");
        //return 0;
    }

    Sender* senderBase = new Sender;

	aiBase * aii = new ai09(state,settings,senderBase);

    debuggerBase * debugger = new debuggerBase(settings,&aii->AIDebug);

    auto grsim_fwd = new GrsimForwarder("127.0.0.1", 20011);

    bool exited = false;
    mutex lock;

	LOG_INFO(" Now it is time, lets rock...");

    auto ai_func = [&]()
    {
        Timer timer;

        while ( (! kbhit()) && ( ImmortalsIsTheBest ) )	//Hope it lasts Forever...
        {
            timer.start();

            vision.recieveAllCameras();

            lock.lock();

            //The vision process
            vision.ProcessVision();
            //The AI process
            aii -> Process( state , settings );

            //TODO #3 comment the GRsim output
//            grsim_fwd->SendData((reinterpret_cast<ai09*>(aii))->OwnRobot, MAX_TEAM_ROBOTS, settings->our_color);

            //The sending process
            senderBase->sendAll();

            //debugging (the visualizer written by python) :
//            debugger->send();

            //LordHippos GUI
//            vision.SendGUIData(state,aii->AIDebug);

            lock.unlock();
            LOG_DEBUG("FPS: {}", 1.0 / timer.interval());
        }
        exited = true;
    };

    auto ref_func = [&]()
    {
        while ( ( !exited ) && (! kbhit()) && ( ImmortalsIsTheBest ) )	//Hope it lasts Forever...
        {
            if ( referee_2018.recieve() )
            {
//                std::cout << "Referre Boz" << std::endl;
                lock.lock();
                referee_2018.process();
                lock.unlock();
//                std::cout << "Referre Boz "<< referee_2018.command_CNT << std::endl;

            }
        }
    };

    auto str_func = [&]()
    {
        UDPSocket* strategyUDP = new UDPSocket(60006);
        strategyUDP -> joinGroup("224.5.23.3");
        const int strategyBufferMaxSize = 100000;
        char strategyBuffer[strategyBufferMaxSize];
        while ( ( !exited ) && (! kbhit()) && ( ImmortalsIsTheBest ) )	//Hope it lasts Forever...
        {
            std::string strategySrcAdd;
            unsigned short strategySrcPort;
            int strategySize = strategyUDP->recvFrom(strategyBuffer, strategyBufferMaxSize, strategySrcAdd, strategySrcPort);
            if ( strategySize > 11 )
            {
                LOG_INFO("Recieved \"strategy.ims\" with size: {} KB, from {} on port {}", float(strategySize)/1000.0f, strategySrcAdd, strategySrcPort);
                lock.lock();
                reinterpret_cast<ai09*>(aii)->read_playBook_str(strategyBuffer, strategySize);
                lock.unlock();
                std::string strategy_path(DATA_DIR); strategy_path.append("/strategy.ims");
                ofstream strategyFile ( strategy_path.c_str() , ios::out|ios::binary);
                strategyFile.write(strategyBuffer, strategySize);
                strategyFile.close();
            }
            else {
                LOG_WARNING("Invalid \"strategy.ims\" received with size: {}", strategySize);
            }
        }

        delete strategyUDP;
    };

    thread ai_thread(ai_func);
    thread ref_thread(ref_func);
    thread str_thread(str_func);

    ai_thread.join();
    ref_thread.join();
    str_thread.join();

	delete settings;
	delete state;
	delete aii;

    Services::shutdown();

	return 0;
}
