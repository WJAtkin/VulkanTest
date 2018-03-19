#include "BUILD_ORDER.h"

int main()
{
	CEngine app;

	try
	{
		app.Run();
	}
	catch (const std::runtime_error& e)
	{
		std::cerr << e.what() << std::endl;
		system("Pause");
		return EXIT_FAILURE;
	}

	system("Pause");
	return EXIT_SUCCESS;
}
