#include "BUILD_ORDER.h"

CLogger::CLogger() { }

void CLogger::LogMessage(std::string msg)
{
#ifdef DEBUG
	std::cout << msg << std::endl;
#endif // DEBUG
}