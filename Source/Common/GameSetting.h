#pragma once
#include "../Reality/Vision/VisionSetting.h"

enum Side { Right , Left };

struct GameSetting
{
	Side side;

	VisionSetting * visionSetting;
};