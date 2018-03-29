#include "BUILD_ORDER.h"

int main()
{
	CEngine* myEngine = new CEngine();

	try
	{
		myEngine->Run();
	}
	catch (const std::runtime_error& e)
	{
		std::cerr << e.what() << std::endl;
		system("Pause");
		return EXIT_FAILURE;
	}

	delete myEngine;
	system("Pause");
	return EXIT_SUCCESS;
}
