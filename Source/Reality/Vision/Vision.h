#pragma once

////////////////////////////////////////////////////////////////////
//  vision.h
//
//  This file contains all the functions necessary for processing
//  raw vision data 
//
////////////////////////////////////////////////////////////////////

#include <iostream>

#include "Protobuf/messages_robocup_ssl_detection.pb.h"
#include "Protobuf/messages_robocup_ssl_geometry.pb.h"
#include "Protobuf/messages_robocup_ssl_wrapper.pb.h"
#include "Protobuf/ImmortalsProtocols.pb.h"

#include "VisionSetting.h"

#include "../../Network/PracticalSocket.h"
#include "Kalman/FilteredObject.h"
#include "../../Common/MedianFilter.h"
#include "../../Common/MATHS_REGRESSION_PARABOLIC.h"
#include "WorldState.h"
#include <zmq.h>

#ifndef INT_MAX
#define INT_MAX       2147483647    /* maximum (signed) int value */
#endif

#ifndef POWED_DIS
#define POWED_DIS(a,b,c,d) (((a-c)*(a-c))+((b-d)*(b-d)))
#endif

#define CAM_COUNT 4

#define PREDICT_STEPS 5.0f

#define MAX_BALLS 10
#define MAX_BALL_NOT_SEEN 40

#define MAX_ROBOT_NOT_SEEN 600
#define MAX_ROBOT_SUBSITUTE 60

#define MERGE_DISTANCE 5000

#define MAX_INCOMING_PACKET_SIZE 1000

#define BALL_BUFFER_FRAMES 30

VisionSetting * _visionSetting ( bool , std::string , short , std::string , short , bool = true , bool = true , bool = true , bool = true );

/**
Class VisionModule : Captures the vision packet from the network, and sends it to the rest
of the AI class.
@author Ali Salehi ( Rewritten from original 2009 source code )
@version 0.1
June 2010
Changes include : 
**/

class VisionModule
{
	public:

		VisionModule ( VisionSetting * );
		~VisionModule();

		VisionSetting * GetSetting ( void );

		bool recievePacket ( void );
		bool connectToVisionServer ( const std::string & , const unsigned short );
		void ProcessVision ( WorldState * );
		bool isConnected ( void );

		void ProcessRobots ( WorldState * );
		int ExtractBlueRobots ( void );
		int ExtractYellowRobots ( void );
		int MergeRobots ( int num );
		void FilterRobots ( int num , bool own );
		void predictRobotsForward( WorldState * );
		void SendStates ( WorldState * );
	
		// Ridemun haie nik injan:
		void InitANN ( void );
		void RunANN ( WorldState * );
		void PredictWithANN ( WorldState * );
		void TrainANN ( float );
		//Tamum shod!

		void SendGUIData ( WorldState * , AI_Debug & );

		void ProcessBalls ( WorldState * );
		int ExtractBalls ( void );
		int MergeBalls ( int num );
		void FilterBalls ( int num , WorldState * );
		void predictBallForward( WorldState * );
		void calculateBallHeight ( void );

	private:

		float ballBufferX[BALL_BUFFER_FRAMES];
		float ballBufferY[BALL_BUFFER_FRAMES];
		int ballBufferIndex;

		//TVec2 lastShootPosition;
		//TVec2 prevBallVel;


		Parabolic ballParabolic;

		FilteredObject ball_kalman;
		FilteredObject robot_kalman[2][MAX_ROBOTS];

		MedianFilter<float> AngleFilter[2][MAX_ROBOTS];
		float rawAngles[2][MAX_ROBOTS];

		VisionSetting * setting;
		bool connected;

		int ball_not_seen;
		int robot_not_seen[2][MAX_ROBOTS];

		SSL_DetectionBall lastRawBall;

		RobotState robotState[2][MAX_ROBOTS];

		char incoming_buffer[MAX_INCOMING_PACKET_SIZE];

		UDPSocket * VisionUDP;
		UDPSocket * GUIUDP;

		void* gui_zmq_context;
		void* gui_zmq_publisher;

		robotDataMsg robotPacket[2][MAX_ROBOTS];

		SSL_WrapperPacket packet;
		SSL_DetectionFrame frame[CAM_COUNT];
		SSL_DetectionBall d_ball[MAX_BALLS*CAM_COUNT];
		SSL_DetectionRobot robot[MAX_ROBOTS*CAM_COUNT];

		bool packet_recieved[CAM_COUNT];

};

