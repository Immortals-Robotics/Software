#pragma once

////////////////////////////////////////////////////////////////////
//  vision.h
//
//  This file contains all the functions necessary for processing
//  raw vision data 
//
////////////////////////////////////////////////////////////////////

#include <iostream>

#include "protos/messages_robocup_ssl_detection.pb.h"
#include "protos/messages_robocup_ssl_geometry.pb.h"

#include "VisionSetting.h"

#include "network/netraw.h"
#include "Kalman/FilteredObject.h"
#include "math/MedianFilter.h"
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

/**
Class Vision : Captures the vision packet from the network, and sends it to the rest
of the AI class.
@author Ali Salehi ( Rewritten from original 2009 source code )
@version 0.1
June 2010
Changes include : 
**/

class Vision
{
	public:
		Vision ( VisionSetting * );
		~Vision();

		const VisionSetting& GetSetting ( void ) const;

		bool Receive ( void );
		bool Open ( const std::string & , const unsigned short );
		void Process ( WorldState& );
		bool IsConnected ( void ) const;
	
		void Publish ( const WorldState& ) const;

	private:
		void ProcessRobots(WorldState &);
		int ExtractBlueRobots(void);
		int ExtractYellowRobots(void);
		int MergeRobots(int num);
		void FilterRobots(int num, bool own);
		void PredictRobots(WorldState &);
		void FillStates(WorldState &);

		void ProcessBalls(WorldState &);
		int ExtractBalls(void);
		int MergeBalls(int num);
		void FilterBalls(int num, WorldState &);
		void PredictBall(WorldState &);
		void ComputeBallHeight(void);

		FilteredObject ball_kalman;
		FilteredObject robot_kalman[2][MAX_ROBOTS];

		MedianFilter<float> AngleFilter[2][MAX_ROBOTS];
		float rawAngles[2][MAX_ROBOTS];

		VisionSetting * setting;
		bool connected;

		unsigned long frameId;

		int ball_not_seen;
		int robot_not_seen[2][MAX_ROBOTS];

		SSL_DetectionBall lastRawBall;

		RobotState robotState[2][MAX_ROBOTS];

		Net::UDP * VisionUDP;

		void* zmq_context;
		void* zmq_publisher;

		SSL_DetectionFrame frame[CAM_COUNT];
		SSL_DetectionBall d_ball[MAX_BALLS*CAM_COUNT];
		SSL_DetectionRobot robot[MAX_ROBOTS*CAM_COUNT];

		bool packet_recieved[CAM_COUNT];

};

