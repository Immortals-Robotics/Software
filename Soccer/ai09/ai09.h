#pragma once

#include "aiBase.h"
#include "Robot.h"
#include "errt.h"
#include "OneTouchDetector.h"
#include "timer.h"

#include <map>

#include <iostream>
using namespace std;

#include "Geom.h"

#include "strategy.pb.h"


class ai09 : public aiBase
{
	private:
	
	int VisionSerialTrans[12];
	
	float penaltyAngle;
	
	bool playingAgainstSkuba;
	int marchingDefender;
	
	float randomParam;
	
	int rmf;
	int gk;
	int def;
	int dmf;
	int lmf;
	int cmf;
	
	int attack;
	int mid1;
	int mid2;
	
	int* stm2AInum[6];
	
	map<int,int> markMap;
	
	int hys;
	
	int lastReferee;
	
	Timer timer;

		bool isDefending;
	
		float openAngleHys;

		float BAR;

		float beta;
		float gamma;
		float shootK;

		bool reached;

		BallState ball;
		RobotState OppRobot[12];
		int OwnRobotNum , OppRobotNum;
		Planner planner[6];
		OneTouchDetector oneTouchDetector[6];
		bool navigated[6];
		int side;
	
		VelocityProfile VELOCITY_PROFILE_AROOM;
		VelocityProfile VELOCITY_PROFILE_KHARAKI;
		VelocityProfile VELOCITY_PROFILE_MAMOOLI;
		void Navigate2Point ( int robot_num , TVec2 dest , bool accurate = false , int speed = 80 , VelocityProfile * velocityProfile = NULL );
		void ERRTNavigate2Point ( int robot_num , TVec2 dest , bool accurate = false , int speed = 80 , VelocityProfile * velocityProfile = NULL );
		TVec2 PointOnConnectingLine(TVec2 FirstPoint,TVec2 SecondPoint,float distance);
		void ERRTSetObstacles ( int robot_num , bool bll = false , bool field = true , bool own = true , bool opp = false , bool dribble = false , bool bigPen = false );

		void Stop ( );
		void stop_ajor ( );
		void NormalPlay ( void );
		void NewNormalPlay ( void );
		void NormalPlayDef ( void );
		void NormalPlayAtt ( void );
		void ManageAttRoles ( void );
		void CalcIsDefending ( void );
		void MarkManager ( void );
		void Mark2Goal ( int robot_num , int opp , float dist = 220.0f );
		void HaltAll ( void );
		void Halt ( int robot_num );
		void GK ( int robot_num = 0 , int defence_num = 2 , bool stop = false );
		void GK_Ghuz ( int robot_num = 0 , int defence_num = 2 , bool stop = false );
		void OneDef ( int robot_num = 1 , TVec2 * defendTarget = NULL , bool stop = false );
		void TwoDef ( int robot_num1 = 1 , int robot_num2 = 2 , TVec2 * defendTarget = NULL );
		void DefGhuz ( int robot_num = 1 , TVec2 * defendTarget = NULL , bool stop = false );
		void PenaltyUs ( int robot_num , float angle , int kick = 0 , int chip = 0 );
		void DefenceWall ( int robot_num , bool kickOff = false );
	
		void corner_simple_pass ( bool cmu = false );
		void corner_simple_chip ( void );
		void corner_switch_pass ( void );
	
		void penalty_us_ghuz ( bool canKickBall );

		void kickoff_us_chip ( bool canKickBall );
		void kickoff_us_pass ( bool canKickBall );
		void kickoff_us_farar ( bool canKickBall );
		void kickoff_us_zamini ( bool canKickBall );
	
		void throwin_chip_shoot ( void );
		void throwin_tu_omgh ( void );
	
		void kickoff_their_one_wall ( void );
		void kickoff_their_two_wall ( void );
		void kickoff_their_back_def ( void );
	
		void penalty_their_simple ( void );
		void penalty_their_gool ( bool canKickBall );

		void throwin_their_khafan ( void );
		void corner_their_khafan ( void );
		void corner_their_two_markers ( void );
		void corner_their_marker_ajor ( void );
		void corner_their_marker_karkas ( void );
		void corner_their_def_karkas ( void );
		void corner_their_def_ajor ( void );
	void corner_their_global ( void );
	
		PlayBook* playBook;
		void read_playBook ( const char* fileName );
		void strategy_maker ( const Strategy& strategy );


		void tech_challenge ( void );
		void tech_mexico ( void );
		void tech_cmu ( void );
	
		float lastBallDirection,lastBallMagnitude;
		TVec2 PredictedBall;
		bool circleReachedBehindBall;	
		void tech_circle ( int robot_num , float angle , int kick = 0 , int chip = 0 , bool needRRT = true , bool gameRestart = false , bool kiss = false , bool dribbler = false );
		
		void WaitForPass ( int robot_num , bool chip = false , TVec2* target = NULL );
		void WaitForOmghi ( int robot_num );

		float calculateOneTouchAngle ( int robot_num , TVec2 oneTouchPosition );

		void SetEvaulateOBJs ( int robot_num1 , int robot_num2 );
		void PassEvaluation ( void );

		TVec2 calculateOpenAngleToGoal ( TVec2 init,int robot_num );
	
		int findCruncherOpp ( int mask1 , int mask2 = -1 , bool acceptNearBall = false );
		int findKickerOpp ( int mask );
		void recievePass ( TVec2 staticPos );
	
		void backPass ( int robot_num , float target , float t );
	
		void dribble ( int robot_num , TVec2 target );
	
		TVec2 predictBallForwardAI( float timeAhead );
		
		void internalProcessData ( WorldState * worldState , GameSetting * setting );
		void internalFinalize ( WorldState * worldState , GameSetting * setting , char * commands );

	public:
		Robot OwnRobot[6];
		ai09 ( void );
		void Process ( WorldState * worldState , GameSetting * setting , char * commands );
};