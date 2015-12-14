#include <iostream>
#include <rglGame.h>

int main(int argc, char** argv)
{
	if (rglGame::getInstance()->run("Example1", 640, 480))
	{
		std::cout << "Finished!\n";
	}
	else
	{
		std::cout << "Could not initialize game.\n";
		system("PAUSE");
	}

	return 0;
}