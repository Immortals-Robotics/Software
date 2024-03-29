#include "ai09.h"

void ai09::InitAIPlayBook ( void )
{
	AIPlayBook.clear();

	AIPlayBook["my_test"] = &ai09::my_test;
	AIPlayBook["NormalPlay"] = &ai09::NormalPlay;
	AIPlayBook["NewNormalPlay"] = &ai09::NewNormalPlay;
	AIPlayBook["NormalPlayDef"] = &ai09::NormalPlayDef;
	AIPlayBook["NormalPlayAtt"] = &ai09::NormalPlayAtt;
	AIPlayBook["Stop"] = &ai09::Stop;
	AIPlayBook["tech_challenge"] = &ai09::tech_challenge;
	AIPlayBook["tech_cmu"] = &ai09::tech_cmu;
	AIPlayBook["tech_mexico"] = &ai09::tech_mexico;
	AIPlayBook["corner_simple_pass"] = &ai09::corner_simple_pass;
	AIPlayBook["corner_simple_chip"] = &ai09::corner_simple_chip;
	AIPlayBook["corner_switch_pass"] = &ai09::corner_switch_pass;
	AIPlayBook["kickoff_us_pass"] = &ai09::kickoff_us_pass;
	AIPlayBook["kickoff_us_farar"] = &ai09::kickoff_us_farar;
	AIPlayBook["kickoff_us_zamini"] = &ai09::kickoff_us_zamini;
	AIPlayBook["kickoff_us_chip"] = &ai09::kickoff_us_chip;
	AIPlayBook["throwin_chip_shoot"] = &ai09::throwin_chip_shoot;
	AIPlayBook["kickoff_their_one_wall"] = &ai09::kickoff_their_one_wall;
	AIPlayBook["kickoff_their_back_def"] = &ai09::kickoff_their_back_def;
	AIPlayBook["corner_their_marker_ajor"] = &ai09::corner_their_marker_ajor;
	AIPlayBook["corner_their_marker_karkas"] = &ai09::corner_their_marker_karkas;
	AIPlayBook["corner_their_def_karkas"] = &ai09::corner_their_def_karkas;
	AIPlayBook["corner_their_def_ajor"] = &ai09::corner_their_def_ajor;
	AIPlayBook["throwin_their_khafan"] = &ai09::throwin_their_khafan;
	AIPlayBook["Stop_def"] = &ai09::Stop_def;
	AIPlayBook["strategy_maker"] = &ai09::strategy_maker;
	AIPlayBook["penalty_us_ghuz"] = &ai09::penalty_us_ghuz;
	AIPlayBook["penalty_us_shootout"] = &ai09::penalty_us_shootout;
	AIPlayBook["penalty_their_gool"] = &ai09::penalty_their_gool;
    AIPlayBook["penalty_their_simple"] = &ai09::penalty_their_simple;
    
	AIPlayBook["throwin_tu_omgh"] = &ai09::throwin_tu_omgh;
	AIPlayBook["corner_their_global"] = &ai09::corner_their_global;
	AIPlayBook["tech_khers_pass"] = &ai09::tech_khers_pass;
	AIPlayBook["tech_khers_def"] = &ai09::tech_khers_def;
	AIPlayBook["corner_their_skuba"] = &ai09::corner_their_skuba;
	AIPlayBook["corner_their_mrl"] = &ai09::corner_their_mrl;
	AIPlayBook["tech_motion_ann"] = &ai09::tech_motion_ann;
    AIPlayBook["throwin_us_outgir"] = &ai09::throwin_us_outgir;
	AIPlayBook["our_place_ball_shoot"] = &ai09::our_place_ball_shoot;
    AIPlayBook["our_place_ball_shoot_V2"] = &ai09::our_place_ball_shoot_V2;
	AIPlayBook["our_place_ball_shoot_taki"] = &ai09::our_place_ball_shoot_taki;
	AIPlayBook["their_place_ball"] = &ai09::their_place_ball;
	AIPlayBook["penalty_our_Shoot_Out"] = &ai09::penalty_our_Shoot_Out;
}