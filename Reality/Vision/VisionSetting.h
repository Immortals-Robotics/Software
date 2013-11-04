#pragma once

#include <iostream>

#define COLOR_BLUE false
#define COLOR_YELLOW true

struct VisionSetting
{
	bool color;

	std::string UDP_Adress;
	short LocalPort;

	std::string GUI_Adress;
	short GUIPort;

	bool use_camera[2];
};