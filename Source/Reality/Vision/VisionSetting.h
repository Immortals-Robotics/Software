#pragma once

#include <iostream>
#include <vector>

#define COLOR_BLUE false
#define COLOR_YELLOW true

struct VisionSetting
{
	bool color;

	std::string UDP_Adress;
	short LocalPort;

	short ZmqPort;

    std::vector<bool> use_camera;
};