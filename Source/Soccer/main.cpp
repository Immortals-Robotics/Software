#include <iostream>
#include <fstream>
#include <thread>
#include <mutex>
#include <string.h>

#include "aiBase.h"
#include "ai09/ai09.h"

#include <timer.h>


using namespace std;

#define ImmortalsIsTheBest true

void initWorldState ( WorldState * state )
{
	state -> balls.Position = Vec2 ( 0.0f );
	state -> balls.velocity.x = 0.0f;
	state -> balls.velocity.y = 0.0f;
	state -> balls.velocity.direction = 0.0f;
	state -> balls.velocity.magnitude = 0.0f;
	state -> balls.seenState = CompletelyOut;
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
		return 0;
	}
	
	
	char rf_freq = 110;
	
	WorldState * state = new WorldState ( );
	initWorldState ( state );
	
	bool started = false;
	
	aiBase * aii;
	aii = new ai09 ( );
	
	
	
	cout << " Now it is time, lets rock..." << endl;
	
	bool exited = false;
	mutex lock;


		auto ai_func = [&]()
		{
			Timer timer;

			while ( ImmortalsIsTheBest )	//Hope it lasts Forever...
			{
				timer.start();
				
                    try {
                        commUDP.sendTo ( robot_cmds    , 77 , "224.5.92.5" , 60005 );
                    } catch (...) {
                        cout << "ERROR: failed to send robot packets." << endl;
                    }
					
                }
				aii -> Process( state , setting , robot_cmds );
				//cout << timer.time() * 1000.0 << endl;
				
				cout << 1.0/timer.interval() << endl;
				
			}
			exited = true;
			commUDP.sendTo ( zeros , 10 , "localhost" , 60001 );
			commUDP.sendTo ( zeros , 1 , "localhost" , 60006 );
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
    thread str_thread(str_func);
	
    ai_thread.join();
    str_thread.join();
    
	delete setting;
	delete state;
	delete aii;
	
	return 0;
}