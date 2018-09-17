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
#include <protos/messages_immortals_configs.pb.h>

#include "network/netraw.h"
#include "Kalman/FilteredObject.h"
#include "math/MedianFilter.h"
#include "WorldState.h"
#include <zmq.h>
#include <protos/messages_immortals_world_state.pb.h>
#include "IdGenerator.h"
#include <memory>

#ifndef INT_MAX
#define INT_MAX       2147483647    /* maximum (signed) int value */
#endif

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
		Vision ( const Immortals::Data::VisionConfig& );
		~Vision();

		const Immortals::Data::VisionConfig& GetSetting ( void ) const;

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
		FilteredObject* robot_kalman[2];

		std::unique_ptr<IdGenerator> blue_id_generator;
		std::unique_ptr<IdGenerator> yellow_id_generator;

		MedianFilter<float>* AngleFilter[2];
		float* rawAngles[2];

		const Immortals::Data::VisionConfig& config;
		bool connected;

		unsigned long frameId;

		int ball_not_seen;
		int* robot_not_seen[2];

		SSL_DetectionBall lastRawBall;

		RobotState* robotState[2];

		Net::UDP * VisionUDP;

		void* zmq_context;
		void* zmq_publisher;

		SSL_DetectionFrame* frame;
		SSL_DetectionBall* d_ball;
		SSL_DetectionRobot* robot;

		bool* packet_received;
};

