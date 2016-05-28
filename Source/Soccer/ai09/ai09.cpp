#include "ai09.h"

using namespace std;

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

ai09::ai09 ( ):maxBallHist(240)
{	
	std::cout << "	Running Immortals SSL AI module 09" << std::endl << "	Hope us luck :D " << std::endl;
	
    field_width = 4500.0f;
    field_height = 3000.0f;
    goal_width = 1000.0f;
    
    penalty_area_r = 1000.0f;
    penalty_area_width = 500.0f;
    
    for ( int i = 0 ; i < 6 ; i ++ )
    {
        oneTouchDetector[i].field_w = field_width;
        oneTouchDetector[i].field_h = field_height;
        
        planner[i].set_field_params(field_width, field_height);
        
        OwnRobot[i].field_w = field_width;
        OwnRobot[i].field_h = field_height;
    }
    
	InitAIPlayBook();
	currentPlay = "HaltAll";
	currentPlayParam = 0;
	
	//for ( int i = 0 ; i < 5 ; i ++ )
	//	OwnRobot[i].serial_id = i + 1;
	
	VisionSerialTrans[0] = 0;
	VisionSerialTrans[1] = 1;
	VisionSerialTrans[2] = 2;
	VisionSerialTrans[3] = 3;
	VisionSerialTrans[4] = 4;
	VisionSerialTrans[5] = 5;
	VisionSerialTrans[6] = 6;
	VisionSerialTrans[7] = 7;
	VisionSerialTrans[8] = 8;
	VisionSerialTrans[9] = 9;
	VisionSerialTrans[10] = 10;
	VisionSerialTrans[11] = 11;
	
	gkIntercepting = false;
	
	playingAgainstSkuba = false;
	marchingDefender = 0;
	
	randomParam = 0.0f;
	target_str = -1;
	
	hys = 0;
	
	reached = false;
	
	isDefending = false;
	oppRestarted = false;
		
	beta = 0.25;	//Damping factor
	gamma = 0.11;	//Reflect factor
	shootK = 7000.0f;
	
	lastReferee = GameState::GAME_OFF;
	
	lmf = 3;
	cmf = 0;
	gk = 2;
	def = 5;
	dmf = 4;
	rmf = 1;
	
	attack = cmf;
	mid1 = rmf;
	mid2 = lmf;
	
	markMap[&dmf] = -1;
	markMap[&mid1] = -1;
	markMap[&mid2] = -1;
	
	stm2AInum[0] = &gk;
	stm2AInum[1] = &def;
	stm2AInum[2] = &dmf;
	stm2AInum[3] = &mid2;
	stm2AInum[4] = &mid1;
	stm2AInum[5] = &attack;
	
	for (int i = 0 ; i < 6 ; i ++ ) {
		oneTouchDetector[i].bState = &ball;
		oneTouchDetector[i].rState = &OwnRobot[i].State;
		oneTouchDetector[i].side = &side;
		
		oneTouchType[i] = oneTouch;
		oneTouchTypeUsed[i] = false;
		
		allafPos[i] = Vec2(0, 0);
	}
	
	for (int i = 0 ; i < 6 ; i ++ )
	{
		OwnRobot[i].set_vision_id(i+1);
		//OwnRobot[i].set_serial_id(VisionSerialTrans[i]);
	}
	OwnRobot[gk].set_vision_id(7);
	OwnRobot[def].set_vision_id(2);
	OwnRobot[dmf].set_vision_id(1);
	OwnRobot[lmf].set_vision_id(3);
	OwnRobot[rmf].set_vision_id(5);
	OwnRobot[cmf].set_vision_id(10);
	
	chip_head = 200;
	
	lastBallDirection = 0;
	lastBallMagnitude = 0;
	circleReachedBehindBall = false;
	PredictedBall = Vec2 ( 0 );
	
	VELOCITY_PROFILE_AROOM.max_spd = Vec2 ( 40.0f );
	VELOCITY_PROFILE_AROOM.max_dec = Vec2 ( 1.0f );
	VELOCITY_PROFILE_AROOM.max_acc = Vec2 ( 0.8f );
	VELOCITY_PROFILE_AROOM.max_w_acc = 40.0f;
	VELOCITY_PROFILE_AROOM.max_w_dec = 140.0f;
	
	VELOCITY_PROFILE_MAMOOLI.max_spd = Vec2 ( 100.0f );
	VELOCITY_PROFILE_MAMOOLI.max_dec = Vec2 ( 2.0f );
	VELOCITY_PROFILE_MAMOOLI.max_acc = Vec2 ( 1.3f );
	VELOCITY_PROFILE_MAMOOLI.max_w_acc = 40.0f;
	VELOCITY_PROFILE_MAMOOLI.max_w_dec = 140.0f;

	
	//VELOCITY_PROFILE_KHARAKI.max_spd = Vec2 ( 60.0f );
	//VELOCITY_PROFILE_KHARAKI.max_dec = Vec2 ( 3.4f );
	//VELOCITY_PROFILE_KHARAKI.max_acc = Vec2 ( 3.0f );
	//VELOCITY_PROFILE_KHARAKI.max_w_acc = 40.0f;
	//VELOCITY_PROFILE_KHARAKI.max_w_dec = 140.0f;
	
	VELOCITY_PROFILE_KHARAKI.max_spd = Vec2 ( 100.0f );
	VELOCITY_PROFILE_KHARAKI.max_dec = Vec2 ( 2.7f );
	VELOCITY_PROFILE_KHARAKI.max_acc = Vec2 ( 1.9f );
	VELOCITY_PROFILE_KHARAKI.max_w_acc = 40.0f;
	VELOCITY_PROFILE_KHARAKI.max_w_dec = 140.0f;

	playBook = NULL;
	string strategy_path(DATA_PATH); strategy_path.append("/strategy.ims");
	read_playBook(strategy_path.c_str());
	if ( playBook )
	{
		//cout << playBook->strategy_size() << " ";
		//cout << playBook->strategy(0).name() << endl;
		
		cout << playBook->strategy_size() << endl;
		/*for ( int i = 0 ; i < playBook->strategy_size() ; i ++ )
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
		}*/
	}
	else {
		cout << "	Coud not open \"strategy.ims\"" << endl;
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

LHP_Frame* ai09::getLFrame ( void )
{
	return &this->lFrame;
}

void swap ( int & a , int & b )
{
	int tmp = a;
	a = b;
	b = tmp;
}

void ai09::read_sharifcup_config ( void )
{
	sharifcup_score_map[1] = 10;
	sharifcup_score_map[13] = 7;
	sharifcup_score_map[5] = -9;
}