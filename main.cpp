#include <iostream>
#include <fstream>
#include <omp.h>
#include <unistd.h>
#include <string.h>
#include <ncurses.h>

#include "GameSetting.h"
#include "Vision.h"
#include "referee.h"
#include "serial.h"
#include "ai09.h"
#include "kbhit.h"
#include "timer.h"
#include "net_log.h"
#include "messages_blob.pb.h"

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
	
	for ( int i = 0 ; i < MAX_ROBOTS ; i ++ )
	{
		state -> OwnRobot[i].Angle = 0.0f;
		state -> OwnRobot[i].AngularVelocity = 0.0f;
		state -> OwnRobot[i].Position = Vec2 ( 0.0f );
		state -> OwnRobot[i].seenState = CompletelyOut;
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
	init_net_log ( "224.5.92.10" , 60090 );
	if (!ImmortalsIsTheBest) {
		cout << "ERROR: Immortals is not the best SSL team anymore." << endl;
		cout << "Shutting down the system..." << endl;
		while(!kbhit());
		return 0;
	}
	
	
	char rf_freq = 110;
	
	GameSetting * setting = new GameSetting ( );
	setting -> visionSetting = _visionSetting ( COLOR_BLUE , "224.5.23.2" , 10002 , "224.5.66.6" , 10009 ,1,1);
	setting -> side = Left;
	
	WorldState * state = new WorldState ( );
	initWorldState ( state );
	
	Referee referee;
	
	referee.init ( "224.5.23.1" , 10001 , setting -> visionSetting -> color );
	cout << " Connecting to RefereeBox server at " << "224.5.23.1" << " , on port : 10001 " << endl;
	if ( !referee.connect ( ) )
	{
		cout << "	Hey you! Put the LAN cable back in its socket, or ..." << endl;
	}
	
	cout << " Connecting to SSL-Vision server at " << "224.5.23.2" << " , on port : 10002 " << endl;
	VisionModule vision ( setting -> visionSetting );
	if ( ! vision.isConnected ( ) )
	{
		cout << "	Hey you! Put the LAN cable back in its socket, or ..." << endl;
		return 0;
	}
	
	UDPSocket commUDP;
	char robot_cmds[90];
	char zeros[90];
	
	for ( int i = 0 ; i < 90 ; i ++ )
	{
		zeros[i] = 0;
		robot_cmds[i]=0;
	}
	
	robot_cmds[66] = 25;
	robot_cmds[77] = 80;
	robot_cmds[78] = rf_freq;
	robot_cmds[84] = rf_freq;
	
	bool started = false;
	
	aiBase * aii;
	aii = new ai09 ( );
	
	Timer timer;
	
	cout << " Now it is time, lets rock..." << endl;
	
	changemode(1);
	
	float gyrOff = 0;
	int offCount = 0;
	
	float vy = 0.0f;
	float sgnvy = 1.5f;
	
	int tid;
	bool exited = false;
	omp_lock_t lock;
	omp_init_lock(&lock);
	omp_set_num_threads(4);
#pragma omp parallel private(tid) shared(exited)
	{
		tid = omp_get_thread_num();
		if ( tid == 0 )
		{
			while ( (! kbhit()) && ( ImmortalsIsTheBest ) )	//Hope it lasts Forever...
			{
				timer.start();
				
				omp_set_lock ( &lock );
				vision.ProcessVision ( state );
				//sleep(1);
				//while (timer.time()*1000.0f<16.6f);//DELAY(100000);
				if ( started )
					commUDP.sendTo ( robot_cmds    , 77 , "224.5.92.5" , 10003 );
				aii -> Process( state , setting , robot_cmds );
				//cout << timer.time() * 1000.0 << endl;
				
				vision.SendGUIData ( state , aii -> AIDebug );
				omp_unset_lock ( &lock );
				
				cout << 1.0/timer.interval() << endl;
				
				started = true;
			}
			exited = true;
			commUDP.sendTo ( zeros , 10 , "localhost" , 60001 );
			commUDP.sendTo ( zeros , 1 , "localhost" , 60006 );
		}
		if ( tid == 1 )
		{
			while ( ( !exited ) && (! kbhit()) && ( ImmortalsIsTheBest ) )	//Hope it lasts Forever...
			{
				if ( referee.recieve() )
				{
					//cout << "Referre Boz" << endl;
					omp_set_lock ( &lock );
					referee.process ( state );
					omp_unset_lock ( &lock );
					//cout << "Referre Boz" << endl;
				}
			}
		}
		if ( tid == 2 )
		{
			UDPSocket* blobUDP = new UDPSocket(60022);
			blobUDP -> joinGroup("224.5.33.35");
			const int blobBufferMaxSize = 100000;
			char blobBuffer[blobBufferMaxSize];
			LHP_Frame* lhp_frame = dynamic_cast<ai09*>(aii)->getLFrame();
			while ( ( !exited ) && (! kbhit()) && ( ImmortalsIsTheBest ) )	//Hope it lasts Forever...
			{
				string blobSrcAdd;
				unsigned short blobSrcPort;
				int blobSize = blobUDP->recvFrom(blobBuffer, blobBufferMaxSize, blobSrcAdd, blobSrcPort);
				omp_set_lock ( &lock );
				lhp_frame->ParseFromArray(blobBuffer, blobSize);
				omp_unset_lock ( &lock );
				cout << "	XXXXXXXXXXXXXXXXXXXXXXXXXXXX: " << lhp_frame->blob_size() << endl;
			}
		}
		if ( tid == 3 )
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
					omp_set_lock ( &lock );
					dynamic_cast<ai09*>(aii)->read_playBook_str(strategyBuffer, strategySize);
					omp_unset_lock ( &lock );
					ofstream strategyFile ( "../../strategy.ims" , ios::out|ios::binary);
					strategyFile.write(strategyBuffer, strategySize);
					strategyFile.close();
				}
				else if ( strategySize == 11 )
				{
					cout << "Recieved PID configs with size: " << float(strategySize) << " B, from " << strategySrcAdd << " on port " << strategySrcPort << "." << endl;
					strategyBuffer[0] = 4;//robot id
					strategyBuffer[1] = 2;
					
					for (int i = 0; i < 11; i ++) {
						cout << (int)((unsigned char)strategyBuffer[i]) << endl;
					}
					commUDP.sendTo ( strategyBuffer , 11 , "224.5.92.5" , 10003 );
				}
				
				else if ( strategySize == 10 )
				{
					cout << "Recieved robot feedback with size: " << float(strategySize) << " B, from " << strategySrcAdd << " on port " << strategySrcPort << "." << endl;
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
		}
	}
	
	changemode(0);
	
	delete setting;
	delete state;
	delete aii;
	
	return 0;
}