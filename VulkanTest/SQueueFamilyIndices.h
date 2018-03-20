#include "BUILD_ORDER.h"
#pragma once

struct SQueueFamilyIndices
{
	int graphicsFamily = -1;
	int presentFamily = -1;
	bool IsComplete();
};