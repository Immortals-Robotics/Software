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

ai09::ai09 ( )
{	
	std::cout << "	Running Immortals SSL AI module 09" << std::endl << "	Hope us luck :D " << std::endl;
	
	//for ( int i = 0 ; i < 5 ; i ++ )
	//	OwnRobot[i].serial_id = i + 1;
	
	VisionSerialTrans[0] = 0;
	VisionSerialTrans[1] = 1;
	VisionSerialTrans[2] = 2;
	VisionSerialTrans[3] = 3;
	VisionSerialTrans[4] = 4;
	VisionSerialTrans[5] = 5;
	VisionSerialTrans[6] = 9;
	VisionSerialTrans[7] = 7;
	VisionSerialTrans[8] = 8;
	VisionSerialTrans[9] = 9;
	VisionSerialTrans[10] = 10;
	VisionSerialTrans[11] = 11;	
	
	playingAgainstSkuba = false;
	marchingDefender = 0;
	
	randomParam = 0.0f;
	
	hys = 0;
	
	reached = false;
	
	isDefending = false;
	
	BAR=89;
	
	beta = 0.0f;	//Damping factor
	gamma = 0.2f;	//Reflect factor
	shootK = 8000.0f;
	
	lastReferee = GameState::GAME_OFF;
	
	openAngleHys = 0;
	
	lmf = 1;
	attack = 0;
	gk = 3;
	def = 4;
	dmf = 2;
	rmf = 5;
	
	markMap[dmf] = -1;
	//markMap[lmf] = -1;
	markMap[rmf] = -1;
	
	stm2AInum[0] = &gk;
	stm2AInum[1] = &def;
	stm2AInum[2] = &dmf;
	stm2AInum[3] = &lmf;
	stm2AInum[4] = &rmf;
	stm2AInum[5] = &attack;
	
	for (int i = 0 ; i < 6 ; i ++ ) {
		oneTouchDetector[i].bState = &ball;
		oneTouchDetector[i].rState = &OwnRobot[i].State;
	}
	
	for (int i = 0 ; i < 6 ; i ++ )
	{
		OwnRobot[i].set_vision_id(i+1);
		//OwnRobot[i].set_serial_id(VisionSerialTrans[i]);
	}
	OwnRobot[gk].set_vision_id(1);
	OwnRobot[def].set_vision_id(2);
	OwnRobot[dmf].set_vision_id(0);
	OwnRobot[lmf].set_vision_id(5);
	OwnRobot[rmf].set_vision_id(3);
	OwnRobot[attack].set_vision_id(6);
	
	
	lastBallDirection = 0;
	lastBallMagnitude = 0;
	circleReachedBehindBall = false;
	PredictedBall = Vec2 ( 0 );
	
	VELOCITY_PROFILE_AROOM.max_spd = Vec2 ( 30.0f );
	VELOCITY_PROFILE_AROOM.max_dec = Vec2 ( 1.3f );
	VELOCITY_PROFILE_AROOM.max_acc = Vec2 ( 1.6f );
	VELOCITY_PROFILE_AROOM.max_w_acc = 40.0f;
	VELOCITY_PROFILE_AROOM.max_w_dec = 140.0f;
	
	VELOCITY_PROFILE_MAMOOLI.max_spd = Vec2 ( 50.0f );
	VELOCITY_PROFILE_MAMOOLI.max_dec = Vec2 ( 1.8f );
	VELOCITY_PROFILE_MAMOOLI.max_acc = Vec2 ( 1.5f );
	VELOCITY_PROFILE_MAMOOLI.max_w_acc = 40.0f;
	VELOCITY_PROFILE_MAMOOLI.max_w_dec = 140.0f;

	
	VELOCITY_PROFILE_KHARAKI.max_spd = Vec2 ( 60.0f );
	VELOCITY_PROFILE_KHARAKI.max_dec = Vec2 ( 3.2f );
	VELOCITY_PROFILE_KHARAKI.max_acc = Vec2 ( 2.9f );
	VELOCITY_PROFILE_KHARAKI.max_w_acc = 40.0f;
	VELOCITY_PROFILE_KHARAKI.max_w_dec = 140.0f;

	playBook = new PlayBook();
	read_playBook("strategy.ims");
	cout << playBook->strategy_size() << " ";
	cout << playBook->strategy(0).name() << endl;
	
	cout << playBook->strategy_size() << endl;
	for ( int i = 0 ; i < playBook->strategy_size() ; i ++ )
	{
		cout << "	" << playBook->strategy(i).role_size() << endl;
		for ( int j = 0 ; j < playBook->strategy(i).role_size() ; j ++ )
		{
			cout << "		" << playBook->strategy(i).role(j).path_size() << endl;
			for (int k = 0 ; k < playBook->strategy(i).role(j).path_size() ; k++ ) {
				cout << "			" << playBook->strategy(i).role(j).path(k).type() << endl;
				cout << "				" << playBook->strategy(i).role(j).path(k).x() << "		" << playBook->strategy(i).role(j).path(k).y() << endl;

			}
		}
	}
	
	cout << playBook->weight_size() << endl;
	for ( int i = 0 ; i < playBook->weight_size() ; i ++ )
	{
		cout << "	" << playBook->weight(i) << endl;
	}		
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