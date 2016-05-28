#pragma once
#include <deque>

#include "../aiBase.h"
#include "Robot.h"
#include "errt/errt.h"
#include "OneTouchDetector.h"
#include "../../Common/timer.h"
#include "../../Common/linear.h"

#include "../../Reality/Vision/Protobuf/messages_blob.pb.h"

#include <map>

#include <iostream>
using namespace std;

#include "Geom.h"

#include "../../Reality/Vision/Protobuf/strategy.pb.h"

#include "../../Common/MedianFilter.h"

class ai09 : public aiBase
{
	private:
	
	Random random;

    float field_width;
    float field_height;
    float goal_width;
    
    float penalty_area_r;
    float penalty_area_width;
    
	map<string,void (ai09::*)()> AIPlayBook;
	string currentPlay;
	uint32_t currentPlayParam;

	void InitAIPlayBook ( void );
	
	bool gkIntercepting;
	
	int chip_head;
	
	int VisionSerialTrans[12];
	
	float penaltyAngle;
    
    int oppGK;
	
	bool playingAgainstSkuba;
	int marchingDefender;
	
	float randomParam;
	int target_str;
	
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
	
	TVec2 allafPos[6];
	
	map<int*,int> markMap;
	
	int hys;
	
	int lastReferee;
	
	Timer timer;
	
	LHP_Frame lFrame;

		bool isDefending;
		bool oppRestarted;
	
		MedianFilter<float> freeAngleFilter[6];
	
		float beta;
		float gamma;
		float shootK;

		bool reached;

		const int maxBallHist;
		deque<BallState> ballHist;
		Linear ballLine;
		BallState ball;
		RobotState OppRobot[12];
		int OwnRobotNum , OppRobotNum;
		Planner planner[6];
	
		OneTouchDetector oneTouchDetector[6];
		enum OneTouchType {
			oneTouch = 0,
			shirje,
			gool,
			allaf
		};
		OneTouchType oneTouchType[6];
		bool oneTouchTypeUsed[6];
	
		bool navigated[6];
		int side;

	bool ballReached(TVec2);

		VelocityProfile VELOCITY_PROFILE_AROOM;
		VelocityProfile VELOCITY_PROFILE_KHARAKI;
		VelocityProfile VELOCITY_PROFILE_MAMOOLI;
	
		// Helpers
		TVec2 PointOnConnectingLine(TVec2 FirstPoint,TVec2 SecondPoint,float distance);
		TVec2 GK_Ghuz ( float predictBallT , float rMul , int def_count = 2);
		TVec2 DefGhuz ( TVec2 * defendTarget = NULL );
		TVec2 CalculatePassPos ( int robot_num , const TVec2& target , const TVec2& statPos , float bar = 89.0f );
		void CalculateBallTrajectory ( void );
		float calculateRobotReachTime ( int robot_num , TVec2 dest , VelocityProfile* vel_profile );
        float calculateBallRobotReachTime ( int robot_num , VelocityProfile* vel_profile );

	
		// boz ha
		void ManageAttRoles ( void );
		void CalcIsDefending ( void );
		void MarkManager ( bool restart = true );
		PlayBook* playBook;
		int strategy_weight ( void );
		float lastBallDirection,lastBallMagnitude;
		TVec2 PredictedBall;
		bool circleReachedBehindBall;	
		float calculateOneTouchAngle ( int robot_num , TVec2 oneTouchPosition );
	
		void SetEvaulateOBJs ( int robot_num1 , int robot_num2 );
		void PassEvaluation ( void );
	
		TVec2 calculateOpenAngleToGoal ( TVec2 init,int robot_num );
	
		bool ballIsGoaling ( void );
		int findNearestAsshole ( TVec2 pos , int mask , bool acceptNearBall = true );
		int findCruncherOpp ( int mask1 , int mask2 = -1 , bool acceptNearBall = false );
		int findKickerOpp ( int mask );
		int findGusheRobot ( int mask );
		int findJeloOpps ( float minX , int* ans , int mask1 , int mask2 , bool acceptNearBall , bool acceptGooshe );
		bool isGooshe ( int id ,bool sameSideAsBall );
		bool goal_blocked ( TVec2 init_pos, float max_shoot_blocker_dis , float shoot_blocker_r );
		bool attackFuckingAngle ( void );
		TVec2 predictBallForwardAI( float timeAhead );
		float oneTouchScore ( int robot_num );
		float calculateOppThreat(int opp, bool restart = false);
		float calculateMarkCost(int robot_num, int opp);
		float calculateSwicthToAttackerScore(int robot_num);

	
		// Skills
		void Navigate2Point ( int robot_num , TVec2 dest , bool accurate = false , int speed = 80 , VelocityProfile * velocityProfile = NULL );
		void ERRTNavigate2Point ( int robot_num , TVec2 dest , bool accurate = false , int speed = 80 , VelocityProfile * velocityProfile = NULL );
		void ERRTSetObstacles ( int robot_num , bool bll = false , bool field = true , bool own = true , bool opp = false , bool dribble = false , bool bigPen = false );
        void AddOppObs ( int mask1 = -1, int mask2 = -1 );
		void Mark(int robot_num, int opp, float dist = 220.0f);
		void Mark2Goal ( int robot_num , int opp , float dist = 220.0f );
		void Mark2Ball ( int robot_num , int opp , float dist = 220.0f );
		void Halt ( int robot_num );
		void GK ( int robot_num = 0 , int defence_num = 2 , bool stop = false );
		void GK_shirje ( int robot_num = 0 );
		void GKHi ( int robot_num = 0 , int defence_num = 2 , bool stop = false );
		void OneDef ( int robot_num = 1 , TVec2 * defendTarget = NULL , bool stop = false );
		void TwoDef ( int robot_num1 = 1 , int robot_num2 = 2 , TVec2 * defendTarget = NULL );
		void DefHi ( int robot_num , TVec2 * defendTarget = NULL , bool stop = false );
		void PenaltyUs ( int robot_num , float angle , int kick = 0 , int chip = 0 );
		void DefenceWall ( int robot_num , bool kickOff = false );
		void tech_circle ( int robot_num , float angle , int kick = 0 , int chip = 0 , bool needRRT = true , bool gameRestart = false , bool kiss = false , bool dribbler = false  , bool needOppRRT = false);
    void intercept_ball ( int robot_num , float angle , int shoot_pow , int chip_pow );
		void WaitForPass ( int robot_num , bool chip = false , TVec2* target = NULL , TVec2* statPos = NULL );
		void WaitForOmghi ( int robot_num , bool chip = false );
		void WaitForGool ( int robot_num , bool chip = false );
		void recievePass ( int robot_num , TVec2 staticPos , bool chip = false );
		void backPass ( int robot_num , float target , float t );
		void dribble ( int robot_num , TVec2 target );
		void circle_ball ( int robot_num , float tagret_angle , int shoot_pow , int chip_pow , float precision, float near_dis_override = -1.0f );
	


		// Plays
		void Stop ( );
		void Stop_def ( );
		void stop_ajor ( );
		void NormalPlay ( void );
		void NewNormalPlay ( void );
		void NormalPlayDef ( void );
		void NormalPlayAtt ( void );
		void HaltAll ( void );
		void corner_simple_pass ( void );
		void corner_simple_chip ( void );
		void corner_switch_pass ( void );
		void penalty_us_ghuz ( void );
		void kickoff_us_chip ( void );
		void kickoff_us_pass ( void );
		void kickoff_us_farar ( void );
		void kickoff_us_zamini ( void );
		void throwin_chip_shoot ( void );
		void throwin_tu_omgh ( void );
		void kickoff_their_one_wall ( void );
		void kickoff_their_two_wall ( void );
		void kickoff_their_back_def ( void );
		void penalty_their_simple ( void );
		void penalty_their_gool ( void );
		void throwin_their_khafan ( void );
		void corner_their_khafan ( void );
		void corner_their_two_markers ( void );
		void corner_their_marker_ajor ( void );
		void corner_their_marker_karkas ( void );
		void corner_their_def_karkas ( void );
		void corner_their_def_ajor ( void );
		void corner_their_global ( void );
		void corner_their_skuba ( void );
		void corner_their_mrl ( void );
		void strategy_maker ( void );
		void tech_challenge ( void );
		void tech_mexico ( void );
		void tech_cmu ( void );
		void tech_khers_pass ( void );
		void tech_khers_def ( void );
		void tech_motion_ann ( void );
		void sharifcup_pre_start ( void );
		void sharifcup_play ( void );
		void sharifcup_play_2nd ( void );
		map<int,int> sharifcup_score_map;
		void sharifcup_play_3rd ( void );
		void sharifcup_play_4th ( void );
		void sharifcup_post_play ( void );
        void throwin_us_outgir ( void );

	
			
		void internalProcessData ( WorldState * worldState , GameSetting * setting );
		void internalFinalize ( WorldState * worldState , GameSetting * setting , char * commands );

	public:
		Robot OwnRobot[6];
		ai09 ( void );
		void Process ( WorldState * worldState , GameSetting * setting , char * commands );
		bool read_playBook ( const char* fileName );
		bool read_playBook_str ( char* buffer , int length );
		LHP_Frame* getLFrame ( void );
		void read_sharifcup_config ( void );

	int WaitToInterceptBall(int state);
};