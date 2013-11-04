#include <iostream>
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
	setting -> side = Right;
	
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
	char robot_cmds[60];
	char zeros[60];
	
	for ( int i = 0 ; i < 60 ; i ++ )
	{
		zeros[i] = 0;
		robot_cmds[i]=0;
	}
	
	/*Serial serial;
	string serialPort = "/dev/tty.PL2303-000013FD";
	int serialBaudRate = 38400;
	cout << " Opening serial port " << serialPort << " with baudrate = " << serialBaudRate << endl;
	serial.Open ( serialPort.c_str() , 38400 );*/
	
	/*string serialPort = "/dev/tty.PL2303-000013FD";
	int serialBaudRate = 38400;
	io_service io;
	cout << " Opening serial port " << serialPort << "with baudrate = " << serialBaudRate << endl;
	serial_port boostSerial( io, serialPort.c_str() );
	if ( !boostSerial.is_open() )
		cout << "	ridi be serial haji!" << endl;
	else
		boostSerial.set_option( serial_port_base::baud_rate( serialBaudRate ) );
	
	boostSerial.cancel();*/
	
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
	omp_set_num_threads(2);
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
				//sleep(1);
				/*if ( state->ball.velocity.magnitude > 7000 )
					cout << state->ball.velocity.magnitude << endl;*/
				
				//robot_cmds[8] = 'a';
				//robot_cmds[9] = 'a';
				/*for (int i = 1; i < 8 ; i++) {
					if (robot_cmds[i]==0) {
						robot_cmds[i] = 1;
					}
					//robot_cmds[i] = max(1,robot_cmds[i]);
				}*/
				//commUDP.sendTo ( robot_cmds    , 10 , "224.5.92.5" , 10003 );
				/*commUDP.sendTo ( robot_cmds+8  , 10 , "224.5.92.5" , 10003 );
				commUDP.sendTo ( robot_cmds+16 , 10 , "224.5.92.5" , 10003 );
				commUDP.sendTo ( robot_cmds+24 , 10 , "224.5.92.5" , 10003 );
				commUDP.sendTo ( robot_cmds+32 , 10 , "224.5.92.5" , 10003 );
				commUDP.sendTo ( robot_cmds+40 , 10 , "224.5.92.5" , 10003 );*/
				//commUDP.sendTo ( "aa" , 2 , "224.5.92.5" , 10003 );
				/*robot_cmds[0] = 0;
				robot_cmds[1] = 6;
				robot_cmds[2] = 1;
				robot_cmds[3] = 1;
				robot_cmds[4] = 1;
				robot_cmds[5] = 1;
				robot_cmds[6] = 1;
				robot_cmds[7] = 1;*/
				//commUDP.sendTo ( robot_cmds , 10 , "192.168.1.208" , 10003 );
				//robot_cmds[0]=0;
				//robot_cmds[8]='a';
				//robot_cmds[9]='a';
				commUDP.sendTo ( robot_cmds    , 10 , "192.168.1.208" , 10003 );
				//commUDP.sendTo ( robot_cmds+8  , 10 , "192.168.1.208" , 10003 );
//				commUDP.sendTo ( robot_cmds+16 , 10 , "192.168.1.208" , 10003 );
//				commUDP.sendTo ( robot_cmds+24 , 10 , "192.168.1.208" , 10003 );
//				commUDP.sendTo ( robot_cmds+32 , 10 , "192.168.1.208" , 10003 );
//				commUDP.sendTo ( robot_cmds+40 , 10 , "192.168.1.208" , 10003 );
				//commUDP.sendTo ( "OverLord!" , 10 , "192.168.1.208" , 10003 );
				//commUDP.sendTo ( "OverLord!" , 10 , "192.168.1.208" , 10003 );
				//commUDP.sendTo ( "sverLord!" , 10 , "192.168.1.208" , 10003 );
				//commUDP.sendTo ( "BodBodBodaaaaaaaaaagh :P" , 24 , "192.168.1.208" , 10003 );
				//serial.Write(robot_cmds, 50);
				timer.start();
				aii -> Process( state , setting , robot_cmds );
				//cout << timer.time() * 1000.0 << endl;
				for ( int i = 0 ; i < 40 ; i ++ )
				{
					/*if ( robot_cmds[i] == 127 )
					 robot_cmds[i] = 128;
					 else if ( robot_cmds[i] == 126 )
					 robot_cmds[i] = 125;*/
				}
				
				vision.SendGUIData ( state , aii -> AIDebug );
				omp_unset_lock ( &lock );
				
				//cout << timer.interval() * 1000.0 << endl;
			}
			exited = true;
			commUDP.sendTo ( zeros , 10 , "localhost" , 10001 );
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
	}
	
	//serial.Close();
	
	changemode(0);
	
	delete setting;
	delete state;
	delete aii;
	
	return 0;
}