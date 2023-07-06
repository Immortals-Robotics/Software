#pragma once

////////////////////////////////////////////////////////////////////
//  vision.h
//
//  This file contains all the functions necessary for processing
//  raw vision data 
//
////////////////////////////////////////////////////////////////////

#include <iostream>

#include "../../Network/Protobuf/messages_robocup_ssl_detection.pb.h"
#include "../../Network/Protobuf/messages_robocup_ssl_geometry.pb.h"
#include "../../Network/Protobuf/messages_robocup_ssl_wrapper.pb.h"
#include "../../Network/Protobuf/ImmortalsProtocols.pb.h"


#include "../../Common/network/udp_client.h"
#include "../../Common/network/udp_server.h"
#include "Kalman/FilteredObject.h"
#include "../../Common/MedianFilter.h"
#include "../../Common/MATHS_REGRESSION_PARABOLIC.h"
#include "../WorldState.h"
#include "../../Common/GameSetting.h"
#include "../../Common/poly_find.h"
#include <deque>

#ifndef INT_MAX
#define INT_MAX       2147483647    /* maximum (signed) int value */
#endif

#ifndef POWED_DIS
#define POWED_DIS(a,b,c,d) (((a-c)*(a-c))+((b-d)*(b-d)))
#endif

#define PREDICT_STEPS 7.0f

#define MAX_BALLS 10
#define MAX_BALL_NOT_SEEN 40

#define MAX_ROBOT_NOT_SEEN 200
#define MAX_ROBOT_SUBSITUTE 60

#define MERGE_DISTANCE 5000

#define BALL_BUFFER_FRAMES 30


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

    VisionModule(GameSetting* settings,WorldState* State);
    ~VisionModule();


    bool recievePacket ( void );
    bool connectToVisionServer ( void );
    void recieveAllCameras( void );
    void ProcessVision ( void );
    bool isConnected ( void );

    void ProcessRobots ( WorldState * );
    int ExtractBlueRobots ( void );
    int ExtractYellowRobots ( void );
    int MergeRobots ( int num );
    void FilterRobots ( int num , bool own );
    void predictRobotsForward( WorldState * );
    void SendStates ( WorldState * );


    void SendGUIData ( WorldState * , AI_Debug & );

    void ProcessBalls ( WorldState * );
    int ExtractBalls ( void );
    int MergeBalls ( int num );
    void FilterBalls ( int num , WorldState * );
    void predictBallForward( WorldState * );
    void calculateBallHeight ( void );

	void ProcessParam ( WorldState * );

private:
	bool our_color;
	bool our_side;

    std::unique_ptr<UdpClient> m_visionUDP;
    std::unique_ptr<UdpClient> m_GUIUDP;

	WorldState* playState;

	bool packet_recieved[Setting::kCamCount];
    TVec2 ball_pos_buff[BALL_BUFFER_FRAMES];

//    int ballBufferIndex;
//    float ballBufferX[BALL_BUFFER_FRAMES];
//    float ballBufferY[BALL_BUFFER_FRAMES];

    SSL_DetectionBall lastRawBall;//The last position of the locked ball
    FilteredObject ball_kalman;
    int ball_not_seen = MAX_BALL_NOT_SEEN + 1;

    RobotState robotState[2][Setting::kMaxRobots];
    FilteredObject robot_kalman[2][Setting::kMaxRobots];
    int robot_not_seen[2][Setting::kMaxRobots];

    MedianFilter<float> AngleFilter[2][Setting::kMaxRobots];
    float rawAngles[2][Setting::kMaxRobots];

    SSL_WrapperPacket packet;
    SSL_DetectionFrame frame[Setting::kCamCount];
    SSL_DetectionBall d_ball[MAX_BALLS*Setting::kCamCount];
    SSL_DetectionRobot robot[Setting::kMaxRobots*Setting::kCamCount];

    robotDataMsg robotPacket[2][Setting::kMaxRobots];

//	double t_capture_buff[MAX_BALLS*Setting::kCamCount];
//    deque<TVec2> ball_dir_buff;
};

