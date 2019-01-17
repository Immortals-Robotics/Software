#pragma once

#include <iostream>
#include "../Referee/game_state.h"
#include "math/Vector.h"
#include <math.h>

#define MAX_ROBOTS 1200
#define MAX_BALLS 1200

enum SeenState { Seen , CompletelyOut , TemprolilyOut };

struct Velocity {
	float x;
	float y;

	float magnitude;

	float direction;
};

struct RobotState
{
	friend std::ostream &operator<< ( std::ostream & oo , const RobotState & state )
	{
		oo << "Robot " << state.vision_id << "		X : " << (int)state.Position.X << "		Y : " << (int)state.Position.Y << "		Tetta : " << (int)state.Angle << std::endl;
		return oo;
	}

	TVec2 Position;
	Velocity velocity;

	bool HasAngle;
	float Angle = 0.0f;
	float AngularVelocity;

	int vision_id;

	SeenState seenState;
    bool OutForSubsitute;
};

struct BallState
{
	friend std::ostream &operator<< ( std::ostream & oo , const BallState & state )
	{
		oo << "Ball " << state.seenState << "		X : " << (int)state.Position.X << "		Y : " << (int)state.Position.Y << std::endl;
		return oo;
	}

	int id;

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
	friend std::ostream &operator<< (std::ostream & oo, const WorldState & state)
	{
		oo << (int)state.has_ball << " balls,	" << state.ownRobots_num << " Own Robots,	" << state.oppRobots_num << " Opp Robots." << std::endl;
		//oo << (int)state.ball.Position.X << "	" << (int)state.ball.Position.Y << std::endl;

		for (int i = 0; i < MAX_ROBOTS; i++)
			if (state.OwnRobot[i].seenState != CompletelyOut)
				oo << state.OwnRobot[i];
		return oo;
	}

	int ownRobots_num, oppRobots_num;
	int balls_num;
	bool has_ball;

	BallState ball;
	BallState balls[MAX_BALLS];

	RobotState OwnRobot[MAX_ROBOTS];
	RobotState OppRobot[MAX_ROBOTS];

	TVec3 lastCMDS[MAX_ROBOTS][11];

	RefereeState refereeState;

	int oppGK;

	WorldState()
	{
		this->refereeState.counter = 0;
		this->refereeState.goals_blue = 0;
		this->refereeState.goals_yellow = 0;
		this->refereeState.time_remaining = 0;
		this->refereeState.State = NULL;

		this->ownRobots_num = 0;
		this->oppRobots_num = 0;

		this->oppGK = -1;

		this->has_ball = false;

		for (int i = 0; i < MAX_BALLS; i++)
		{
			this->balls[i].Position = Vec2(0.0f);
			this->balls[i].velocity.x = 0.0f;
			this->balls[i].velocity.y = 0.0f;
			this->balls[i].velocity.direction = 0.0f;
			this->balls[i].velocity.magnitude = 0.0f;
			this->balls[i].seenState = CompletelyOut;
		}

		for (int i = 0; i < MAX_ROBOTS; i++)
		{
			this->OwnRobot[i].Angle = 0.0f;
			this->OwnRobot[i].AngularVelocity = 0.0f;
			this->OwnRobot[i].Position = Vec2(0.0f);
			this->OwnRobot[i].seenState = CompletelyOut;
			this->OwnRobot[i].OutForSubsitute = true;
			this->OwnRobot[i].velocity.direction = 0.0f;
			this->OwnRobot[i].velocity.magnitude = 0.0f;
			this->OwnRobot[i].velocity.x = 0.0f;
			this->OwnRobot[i].velocity.y = 0.0f;
			this->OwnRobot[i].vision_id = i;
			for (int j = 0; j < 11; j++)
			{
				this->lastCMDS[i][j] = Vec3(0.0f);
			}
		}
	}
};