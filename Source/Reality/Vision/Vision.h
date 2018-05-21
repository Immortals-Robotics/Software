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


#include "../../Network/PracticalSocket.h"
#include "Kalman/FilteredObject.h"
#include "../../Common/MedianFilter.h"
#include "../../Common/MATHS_REGRESSION_PARABOLIC.h"
#include "../WorldState.h"
#include "../../Common/GameSetting.h"
#include <zmq.h>

#ifndef INT_MAX
#define INT_MAX       2147483647    /* maximum (signed) int value */
#endif

#ifndef POWED_DIS
#define POWED_DIS(a,b,c,d) (((a-c)*(a-c))+((b-d)*(b-d)))
#endif

#define CAM_COUNT 8

#define PREDICT_STEPS 7.0f

#define MAX_BALLS 10
#define MAX_BALL_NOT_SEEN 40

#define MAX_ROBOT_NOT_SEEN 600
#define MAX_ROBOT_SUBSITUTE 60

#define MERGE_DISTANCE 5000

#define MAX_INCOMING_PACKET_SIZE 1000

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

private:
	bool our_color;
	bool our_side;

	std::string vision_UDP_Address;
	short visionPort;

	std::vector<bool> use_camera;

	bool connected;

	UDPSocket * visionUDP;
	UDPSocket * GUIUDP;

	WorldState* playState;

	bool packet_recieved[CAM_COUNT];
    TVec2 ball_pos_buff[BALL_BUFFER_FRAMES];
    char incoming_buffer[MAX_INCOMING_PACKET_SIZE];

//    int ballBufferIndex;
//    float ballBufferX[BALL_BUFFER_FRAMES];
//    float ballBufferY[BALL_BUFFER_FRAMES];

    SSL_DetectionBall lastRawBall;//The last position of the locked ball
    FilteredObject ball_kalman;
    int ball_not_seen;

    RobotState robotState[2][MAX_ROBOTS];
    FilteredObject robot_kalman[2][MAX_ROBOTS];
    int robot_not_seen[2][MAX_ROBOTS];

    MedianFilter<float> AngleFilter[2][MAX_ROBOTS];
    float rawAngles[2][MAX_ROBOTS];

    void* gui_zmq_context;
    void* gui_zmq_publisher;

    SSL_WrapperPacket packet;
    SSL_DetectionFrame frame[CAM_COUNT];
    SSL_DetectionBall d_ball[MAX_BALLS*CAM_COUNT];
    SSL_DetectionRobot robot[MAX_ROBOTS*CAM_COUNT];

    robotDataMsg robotPacket[2][MAX_ROBOTS];
};

