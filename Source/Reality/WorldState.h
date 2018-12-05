#pragma once

#include <iostream>
#include "../Reality/Referee_2018/game_state.h"
#include "../Common/Vector.h"
#include "../Common/poly_find.h"
#include <math.h>
#include <fstream>

#define MAX_ROBOTS 12//The variety of standard patterns that we can have is 12
#define MAX_TEAM_ROBOTS 8

#ifndef WORLDSTATEHDR
#define WORLDSTATEHDR

enum SeenState { Seen , CompletelyOut , TemprolilyOut };

//This part is for getting some samples for filter designs:
struct sample_tuple{
    float t;
    float X;
    float Y;
    float VX_in;
    float VY_in;
    float VX_out;
    float VY_out;

};


void write_to_file();
void get_sample_set1(double sample_t, float sample_X,float sample_Y);
void get_sample_set2(float sample_VX, float sample_VY);
void get_sample_set3(float sample_VX, float sample_VY);
//--------------------------------------

struct Velocity {
	float x;
	float y;

	float magnitude;
	float direction;
};

struct RobotState
{
    int vision_id;

	TVec2 Position;
	Velocity velocity;

	float Angle = 0.0f;
	float AngularVelocity;

	SeenState seenState;
    bool OutForSubsitute;

    friend std::ostream &operator<< ( std::ostream & oo , const RobotState & state ) {
        oo << "Robot " << state.vision_id << "		X : " << (int)state.Position.X << "		Y : " << (int)state.Position.Y << "		Tetta : " << (int)state.Angle << std::endl;
        return oo;
    }
};

struct BallState
{
	TVec2 Position;
	Velocity velocity;

	SeenState seenState;

    TVec2 First_Pos_when_shooted;
    TVec2 path_dir;
    double t_capture;

    friend std::ostream &operator<< ( std::ostream & oo , const BallState & state ) {
        oo << "Ball " << state.seenState << "		X : " << (int)state.Position.X << "		Y : " << (int)state.Position.Y << std::endl;
        return oo;
    }
};

struct RefereeState
{
	GameState * State;
	unsigned short time_remaining;
	unsigned short goals_blue;
	unsigned short goals_yellow;
	unsigned char counter;
    TVec2 placeBallTargetPosition;
    int oppGK;
};

struct WorldState
{

	int ownRobots_num , oppRobots_num;
	bool has_ball;

	BallState ball;

	RobotState OwnRobot[MAX_ROBOTS];
	RobotState OppRobot[MAX_ROBOTS];

	TVec3 lastCMDS[MAX_ROBOTS][11];

	RefereeState* refereeState;

    double t_capture = -1;
    double delta_t_capture = -1;


    WorldState(){
        ball.Position = Vec2 ( 0.0f );
        ball.velocity.x = 0.0f;
        ball.velocity.y = 0.0f;
        ball.velocity.direction = 0.0f;
        ball.velocity.magnitude = 0.0f;
        ball.t_capture = 0.0f;
        ball.seenState = CompletelyOut;
        has_ball = false;

        refereeState = new RefereeState;
        refereeState->State = new GameState;
        refereeState->time_remaining = 0;
        refereeState->goals_blue = 0;
        refereeState->goals_yellow = 0;
        refereeState->counter = 0;
        refereeState->placeBallTargetPosition = Vec2( 0.0f );

        ownRobots_num = 0;
        for ( int i = 0 ; i < MAX_ROBOTS ; i ++ ){
            OwnRobot[i].Angle = 0.0f;
            OwnRobot[i].AngularVelocity = 0.0f;
            OwnRobot[i].Position = Vec2 ( 0.0f );
            OwnRobot[i].seenState = CompletelyOut;
            OwnRobot[i].OutForSubsitute = true;
            OwnRobot[i].velocity.direction = 0.0f;
            OwnRobot[i].velocity.magnitude = 0.0f;
            OwnRobot[i].velocity.x = 0.0f;
            OwnRobot[i].velocity.y = 0.0f;
            OwnRobot[i].vision_id = i;
            for ( int j = 0 ; j < 11 ; j ++ )
            {
                lastCMDS[i][j] = Vec3 ( 0.0f );
            }
        }
        oppRobots_num = 0;
        for ( int i = 0 ; i < MAX_ROBOTS ; i ++ ){//TODO not in the last code...
            OppRobot[i].Angle = 0.0f;
            OppRobot[i].AngularVelocity = 0.0f;
            OppRobot[i].Position = Vec2 ( 0.0f );
            OppRobot[i].seenState = CompletelyOut;
            OppRobot[i].OutForSubsitute = true;
            OppRobot[i].velocity.direction = 0.0f;
            OppRobot[i].velocity.magnitude = 0.0f;
            OppRobot[i].velocity.x = 0.0f;
            OppRobot[i].velocity.y = 0.0f;
            OppRobot[i].vision_id = i;
        }

    }


    friend std::ostream &operator<< ( std::ostream & oo , const WorldState & state ) {
        oo << (int)state.has_ball <<" balls,	" << state.ownRobots_num << " Own Robots,	" << state.oppRobots_num << " Opp Robots." << std::endl;
        oo << (int)state.ball.Position.X << "	" << (int)state.ball.Position.Y << std::endl;

        for ( int i = 0 ; i < MAX_ROBOTS ; i ++ )
            if ( state.OwnRobot[i].seenState != CompletelyOut )
                oo << state.OwnRobot[i] ;
        return oo;
    }
};

#endif //WORLDSTATE_HDR