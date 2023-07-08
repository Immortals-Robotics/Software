#include "ai09.h"

#include "helpers.h"
#include <stdlib.h>



struct RobotProperty
{
	int serialID;
	bool oldRobot;
	bool hasKick;
	bool hasChip;
	bool hasDribble;
};

ai09::ai09(WorldState *_worldState, GameSetting *_setting, Sender* _sender):maxBallHist(240),
																			worldState(_worldState),
																			settings(_setting),
																			senderBase(_sender),
																			cmf(0),rmf(1),lmf(2),gk(3),
																			dmf(4),def(5),rw(6),lw(7)
{
	std::cout << "	Running Immortals SSL AI module 09" << std::endl << "	Hope us luck :D " << std::endl;

#ifndef NEW_FIELD_2018
    field_width = 4500.0f;
    field_height = 3000.0f;
    goal_width = 1000.0f;

    penalty_area_r = 1000.0f;
    penalty_area_width = 500.0f;
#else
	field_width = 6000.0f;// The new field is here:
    field_height = 4500.0f;
    goal_width = 1800.0f;//TODO #6 this was 1000 (!!!) Changed it back

    penalty_area_r = 1800.0f;
    penalty_area_width = 3600.0f;
#endif


    REF_playState = _worldState->refereeState->State;
	targetBallPlacement = &_worldState->refereeState->placeBallTargetPosition;

    for ( int i = 0 ; i < Setting::kMaxOnFieldTeamRobots ; i ++ )
    {
        oneTouchDetector[i].field_w = field_width;
        oneTouchDetector[i].field_h = field_height;

        planner[i].set_field_params(field_width, field_height);

        OwnRobot[i].field_w = field_width;
        OwnRobot[i].field_h = field_height;
    }

    dss = new Dss(OwnRobot, OppRobot, 92.f, 1.f/61.57f, 7000.f, 3000.f);

	InitAIPlayBook();
	currentPlay = "HaltAll";
	currentPlayParam = 0;

	gkIntercepting = false;

	randomParam = 0.0f;
	target_str = -1;

	reached = false;

	isDefending = false;
	oppRestarted = false;

	beta = 0.4;	//Damping factor
	gamma = 0.14;	//Reflect factor
	shootK = 4000.0f;

	lastReferee = GameState::GAME_OFF;

	attack = cmf;
	mid1 = rmf;
	mid2 = lmf;

	markMap[&dmf] = -1;
	markMap[&mid1] = -1;
	markMap[&mid2] = -1;
	markMap[&lw] = -1;
	markMap[&rw] = -1;

	stm2AInum[0] = &gk;
	stm2AInum[1] = &def;
	stm2AInum[2] = &dmf;
	stm2AInum[3] = &mid2;
	stm2AInum[4] = &mid1;
	stm2AInum[5] = &attack;
    stm2AInum[6] = &rw;
    stm2AInum[7] = &lw;
//    std::cout<<"THE GK_pointer: "<<&gk <<std::endl;
//    std::cout<<"THE GK_pointer: "<<stm2AInum[0] <<std::endl;

	for (int i = 0 ; i < Setting::kMaxOnFieldTeamRobots; i ++ ) {
		oneTouchDetector[i].bState = &ball;
		oneTouchDetector[i].rState = &OwnRobot[i].State;
		oneTouchDetector[i].side = &side;

		oneTouchType[i] = oneTouch;
		oneTouchTypeUsed[i] = false;

		allafPos[i] = Vec2(0, 0);
	}

	for (int i = 0 ; i < Setting::kMaxOnFieldTeamRobots; i ++ )
	{
		OwnRobot[i].set_vision_id(i+1);
	}
    OwnRobot[gk].set_vision_id(1);
    OwnRobot[def].set_vision_id(2);
    OwnRobot[dmf].set_vision_id(4);
    OwnRobot[lmf].set_vision_id(0);
    OwnRobot[rmf].set_vision_id(3);
    OwnRobot[cmf].set_vision_id(7);
	OwnRobot[rw].set_vision_id(10);
	OwnRobot[lw].set_vision_id(11);
	//TODO comment this (used fogrSim)
//	OwnRobot[gk].set_vision_id(0);
//    OwnRobot[def].set_vision_id(1);
//    OwnRobot[dmf].set_vision_id(2);
//    OwnRobot[lmf].set_vision_id(3);
//    OwnRobot[rmf].set_vision_id(4);
//    OwnRobot[cmf].set_vision_id(5);
//	OwnRobot[rw].set_vision_id(6);
//	OwnRobot[lw].set_vision_id(7);


	chip_head = 200;

	lastBallDirection = 0;
	lastBallMagnitude = 0;
	circleReachedBehindBall = false;
	PredictedBall = Vec2 ( 0 );

	for(int i=0;i<Setting::kMaxOnFieldTeamRobots;i++)
		requiredRobots[i]= false;

    BALL_PLACE_KHEYLI_SOOSKI.max_spd = Vec2 ( 10.0f );
    BALL_PLACE_KHEYLI_SOOSKI.max_dec = Vec2 ( 1.0f );
    BALL_PLACE_KHEYLI_SOOSKI.max_acc = Vec2 ( 0.6f );
    BALL_PLACE_KHEYLI_SOOSKI.max_w_acc = 40.0f;
    BALL_PLACE_KHEYLI_SOOSKI.max_w_dec = 140.0f;

	VELOCITY_PROFILE_AROOM.max_spd = Vec2 ( 20.0f );
	VELOCITY_PROFILE_AROOM.max_dec = Vec2 ( 1.0f );
	VELOCITY_PROFILE_AROOM.max_acc = Vec2 ( 0.8f );
	VELOCITY_PROFILE_AROOM.max_w_acc = 40.0f;
	VELOCITY_PROFILE_AROOM.max_w_dec = 140.0f;

	VELOCITY_PROFILE_MAMOOLI.max_spd = Vec2 ( 35.0f );
	VELOCITY_PROFILE_MAMOOLI.max_dec = Vec2 ( 1.5f );
	VELOCITY_PROFILE_MAMOOLI.max_acc = Vec2 ( 1.0f );
	VELOCITY_PROFILE_MAMOOLI.max_w_acc = 40.0f;
	VELOCITY_PROFILE_MAMOOLI.max_w_dec = 140.0f;

	VELOCITY_PROFILE_KHARAKI.max_spd = Vec2 ( 35.0f );
	VELOCITY_PROFILE_KHARAKI.max_dec = Vec2 ( 1.5f );
	VELOCITY_PROFILE_KHARAKI.max_acc = Vec2 ( 1.0f );
	VELOCITY_PROFILE_KHARAKI.max_w_acc = 40.0f;
	VELOCITY_PROFILE_KHARAKI.max_w_dec = 140.0f;


	playBook = NULL;
	std::string strategy_path(DATA_DIR); strategy_path.append("/strategy.ims");
	read_playBook(strategy_path.c_str());
	if ( playBook )
	{
#if 1
		LOG_INFO("Strategy loaded with size {}", playBook->strategy_size());
#else
		std::cout << playBook->strategy_size() << " ";
		std::cout << playBook->strategy(0).name() << std::endl;

		std::cout << playBook->strategy_size() << std::endl;
		for ( int i = 0 ; i < playBook->strategy_size() ; i ++ )
		{
			std::cout << "	" << playBook->strategy(i).role_size() << std::endl;
			for ( int j = 0 ; j < playBook->strategy(i).role_size() ; j ++ )
			{
				std::cout << "		" << playBook->strategy(i).role(j).path_size() << std::endl;
				for (int k = 0 ; k < playBook->strategy(i).role(j).path_size() ; k++ ) {
					std::cout << "			" << playBook->strategy(i).role(j).path(k).type() << std::endl;
					std::cout << "				" << playBook->strategy(i).role(j).path(k).x() << "		" << playBook->strategy(i).role(j).path(k).y() << std::endl;

				}
			}
		}

		std::cout << playBook->weight_size() << std::endl;
		for ( int i = 0 ; i < playBook->weight_size() ; i ++ )
		{
			std::cout << "	" << playBook->weight(i) << std::endl;
		}
#endif
	}
	else {
		LOG_CRITICAL("Could not open \"strategy.ims\"");
	}

	timer.start();

}

TVec2 ai09::PointOnConnectingLine(TVec2 FirstPoint,TVec2 SecondPoint,float distance){
	float m = (SecondPoint.Y-FirstPoint.Y)/(SecondPoint.X-FirstPoint.X);
	TVec2 ans;
	if(SecondPoint.X-FirstPoint.X>0)
		ans.X = FirstPoint.X + distance/sqrt(pow(m,2)+1);
	else
		ans.X = FirstPoint.X - distance/sqrt(pow(m,2)+1);
	ans.Y = FirstPoint.Y + m*(ans.X-FirstPoint.X);
	return ans;
}

void swap ( int & a , int & b )
{
	int tmp = a;
	a = b;
	b = tmp;
}

