#pragma once
//#include "../Reality/Vision/VisionSetting.h"



#include <iostream>
#include <vector>

#define COLOR_BLUE false
#define COLOR_YELLOW true

#define RIGHT_SIDE true
#define LEFT_SIDE false

struct GameSetting
{
	bool our_color;
	bool our_side;

	std::string vision_UDP_Address;
	short visionPort;

	std::string referee_UDP_Address;
	short refereePort;

	std::string GUI_UDP_Address;
	short GUIPort;

	std::vector<bool> use_camera;

	unsigned char rf_frq;
};