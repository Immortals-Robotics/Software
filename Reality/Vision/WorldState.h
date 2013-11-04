#pragma once

#include <iostream>
#include "game_state.h"
#include "Vector.h"
#include <math.h>

#define MAX_ROBOTS 12

enum SeenState { Seen , CompletelyOut , TemprolilyOut };

struct Velocity {
	float x;
	float y;

	float magnitude;

	float direction;
};

//Replaced with RobotState
/*struct RobotTarget
{
	float x;
	float y;

	Velocity velocity;

	float angle;
};*/

struct RobotState
{
	friend std::ostream &operator<< ( std::ostream & oo , const RobotState & state )
	{
		oo << "Robot " << state.vision_id << "		X : " << (int)state.Position.X << "		Y : " << (int)state.Position.Y << "		Tetta : " << (int)state.Angle << std::endl;
		return oo;
	}

	TVec2 Position;
	Velocity velocity;

	float Angle;
	float AngularVelocity;

	int vision_id;

	SeenState seenState;
};

struct BallState
{
	friend std::ostream &operator<< ( std::ostream & oo , const BallState & state )
	{
		oo << "Ball " << state.seenState << "		X : " << (int)state.Position.X << "		Y : " << (int)state.Position.Y << std::endl;
		return oo;
	}
	TVec2 Position;
	Velocity velocity;

	SeenState seenState;

};

struct RefereeState
{
	GameState * State;
	unsigned short time_remaining;
	unsigned short goals_blue;
	unsigned short goals_yellow;
	unsigned char counter;
};

struct WorldState
{
	friend std::ostream &operator<< ( std::ostream & oo , const WorldState & state )
	{
		oo << (int)state.has_ball <<" balls,	" << state.ownRobots_num << " Own Robots,	" << state.oppRobots_num << " Opp Robots." << std::endl;
		oo << (int)state.ball.Position.X << "	" << (int)state.ball.Position.Y << std::endl;

		for ( int i = 0 ; i < MAX_ROBOTS ; i ++ )
			if ( state.OwnRobot[i].seenState != CompletelyOut )
			oo << state.OwnRobot[i] ;
		return oo;
	}

	int ownRobots_num , oppRobots_num;
	bool has_ball;

	BallState ball;

	RobotState OwnRobot[MAX_ROBOTS];
	RobotState OppRobot[MAX_ROBOTS];

	TVec3 lastCMDS[MAX_ROBOTS][10];

	RefereeState refereeState;
};