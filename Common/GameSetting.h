#pragma once
#include "VisionSetting.h"

enum Side { Right , Left };

struct GameSetting
{
	Side side;

	VisionSetting * visionSetting;
};