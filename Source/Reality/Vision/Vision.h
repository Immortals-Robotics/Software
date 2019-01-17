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
		void PredictRobots();
		void FillRobotStates(WorldState &);

		void ProcessBalls(WorldState &);
		int ExtractBalls(void);
		int MergeBalls(int num);
		void FilterBalls(int num, const std::vector<int>& ball_ids);
		void PredictBall();
		void FillBallStates(WorldState & state);
		void ComputeBallHeight(void);

		void ResetState(WorldState& state);

		std::vector<FilteredObject> ball_kalman;
		std::vector<FilteredObject> robot_kalman[2];

		std::unique_ptr<IdGenerator> blue_id_generator;
		std::unique_ptr<IdGenerator> yellow_id_generator;
		std::unique_ptr<IdGenerator> ball_id_generator;

		std::vector<MedianFilter<float>> AngleFilter[2];
		std::vector<float> rawAngles[2];

		const Immortals::Data::VisionConfig& config;
		bool connected;

		unsigned long frameId;

		std::vector<int> ball_not_seen;
		std::vector<int> robot_not_seen[2];

		std::vector<BallState> ballState;
		std::vector<RobotState> robotState[2];

		std::unique_ptr<Net::UDP> VisionUDP;

		void* zmq_context;
		void* zmq_publisher;

		std::vector<SSL_DetectionFrame> frame;
		std::vector<SSL_DetectionBall> d_ball;
		std::vector<SSL_DetectionRobot> robot;

		unsigned capture_id;

		std::vector<bool> packet_received;
};

