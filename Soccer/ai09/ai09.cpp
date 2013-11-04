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
	
	VisionSerialTrans[0] = 15;
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
	
	playingAgainstSkuba = false;
	marchingDefender = 0;
	
	randomParam = 0.0f;
	
	hys = 0;
	
	reached = false;
	
	isDefending = false;
	
	BAR=0;
	
	beta = 0.1f;
	gamma = 0.5f;
	shootK = 8300.0f;
	
	lastReferee = GameState::GAME_OFF;
	
	openAngleHys = 0;
	
	passgir = 1;
	attack = 0;
	gk = 3;
	def1 = 4;
	def2 = 2;
	
	for (int i = 0 ; i < 5 ; i ++ )
	{
		OwnRobot[i].set_vision_id(i+1);
		//OwnRobot[i].set_serial_id(VisionSerialTrans[i]);
	}
	OwnRobot[gk].set_vision_id(6);
	OwnRobot[def1].set_vision_id(4);
	OwnRobot[def2].set_vision_id(2);
	OwnRobot[passgir].set_vision_id(0);
	OwnRobot[attack].set_vision_id(3);
	
	
	lastBallDirection = 0;
	lastBallMagnitude = 0;
	circleReachedBehindBall = false;
	PredictedBall = Vec2 ( 0 );
	
	VELOCITY_PROFILE_AROOM.max_spd = Vec2 ( 50.0f );
	VELOCITY_PROFILE_AROOM.max_dec = Vec2 ( 1.3f );
	VELOCITY_PROFILE_AROOM.max_acc = Vec2 ( 1.6f );
	VELOCITY_PROFILE_AROOM.max_w_acc = 40.0f;
	VELOCITY_PROFILE_AROOM.max_w_dec = 140.0f;
	
	VELOCITY_PROFILE_MAMOOLI.max_spd = Vec2 ( 100.0f );
	VELOCITY_PROFILE_MAMOOLI.max_dec = Vec2 ( 2.3f );
	VELOCITY_PROFILE_MAMOOLI.max_acc = Vec2 ( 1.9f );
	VELOCITY_PROFILE_MAMOOLI.max_w_acc = 40.0f;
	VELOCITY_PROFILE_MAMOOLI.max_w_dec = 140.0f;

	
	VELOCITY_PROFILE_KHARAKI.max_spd = Vec2 ( 100.0f );
	VELOCITY_PROFILE_KHARAKI.max_dec = Vec2 ( 4.3f );
	VELOCITY_PROFILE_KHARAKI.max_acc = Vec2 ( 3.6f );
	VELOCITY_PROFILE_KHARAKI.max_w_acc = 40.0f;
	VELOCITY_PROFILE_KHARAKI.max_w_dec = 140.0f;


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