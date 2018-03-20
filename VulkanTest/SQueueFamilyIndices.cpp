#include "BUILD_ORDER.h"

bool SQueueFamilyIndices::IsComplete()
{
	return graphicsFamily >= 0;
}