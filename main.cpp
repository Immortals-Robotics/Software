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

#include <stdio.h>   /* Standard input/output definitions */
#include <string.h>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */

using namespace std;

#define ImmortalsIsTheBest true

/*void SendSerial ( Serial & serial , char * data , int length )
{
	for ( int i = 0 ; i < length ; i ++ )
	{
		//serial.Write ( data+i , 1 );
		sleep ( 1 );
	}
}*/

void haltAll ( char * cmds )
{
	for ( int i = 0 ; i < 5 ; i ++ )
	{
		cmds[8*i+0] = 0;
		cmds[8*i+1] = i+1;
		cmds[8*i+2] = 128;
		cmds[8*i+3] = 128;
		cmds[8*i+4] = 128;
		cmds[8*i+5] = 128;
		cmds[8*i+6] = 4;
		cmds[8*i+7] = 2;
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
	GameSetting * setting = new GameSetting ( );
	setting -> visionSetting = _visionSetting ( COLOR_BLUE , "224.5.23.2" , 10002 , "224.5.66.6" , 10009 ,1,1);
	setting -> side = Left;
	
	WorldState * state = new WorldState ( );
	initWorldState ( state );
	
	Referee referee;
	
	referee.init ( "224.5.23.1" , 60001 , setting -> visionSetting -> color );
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
		//while ( ! vision.isConnected ( ) )
		//	vision.connectToVisionServer ( vision.GetSetting ( ) -> UDP_Adress , vision.GetSetting ( ) -> LocalPort );
	}
	
	UDPSocket commUDP;
	char robot_cmds[80];
	char zeros[80];
	
	for ( int i = 0 ; i < 80 ; i ++ )
	{
		zeros[i] = 0;
		robot_cmds[i]=0;
	}
	
	robot_cmds[66] = 25;
	
	bool started = false;
	
	aiBase * aii;
	aii = new ai09 ( );
	
	Timer timer;
	
	cout << " Now it is time, lets rock..." << endl;
	
	/*initscr();			// Start curses mode 		  
	printw("Hello World !!!");	// Print Hello World		  
	refresh();			// Print it on to the real screen 
	getch();			// Wait for user input 
	endwin();			// End curses mode		  */
	/*int fd = open_port();
	for (int i = 0 ; i < 1000 ; i++ ) {
		int n = write(fd, "ATZ", 3);
		if (n < 0)
			fputs("write() of 4 bytes failed!\n", stderr);
		else
			fputs("write() of 4 bytes ok!\n", stderr);
	}*/
	
	changemode(1);
	
	int tid;
	bool exited = false;
	omp_lock_t lock;
	omp_init_lock(&lock);
	omp_set_num_threads(3);
#pragma omp parallel private(tid) shared(exited)
	{
		tid = omp_get_thread_num();
		if ( tid == 0 )
		{
			while ( (! kbhit()) && ( ImmortalsIsTheBest ) )	//Hope it lasts Forever...
			{
				//timer.start();
				
				omp_set_lock ( &lock );
				vision.ProcessVision ( state );
				if ( started )
				{
					commUDP.sendTo ( robot_cmds    , 77 , "224.5.92.5" , 10003 );
				}
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
					ofstream strategyFile ( "strategy.ims" , ios::out|ios::binary);
					strategyFile.write(strategyBuffer, strategySize);
					strategyFile.close();
				}
				else if ( strategySize == 11 )
				{
					cout << "Recieved PID configs with size: " << float(strategySize) << " B, from " << strategySrcAdd << " on port " << strategySrcPort << "." << endl;
					strategyBuffer[0] = robot_cmds[0];
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
					}
					cout << endl;*/
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