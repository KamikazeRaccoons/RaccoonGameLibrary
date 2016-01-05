#include <iostream>
#include <rglGame.h>
#include <rglDebugger.h>
#include "ExampleState.h"

int main(int argc, char** argv)
{
	if (!rglGame::getInstance()->run("Example1", 640, 480, make_shared<ExampleState>()))
	{
		rglDebugger::log("Could not initialize game.\n", rglDebugger::ERROR);
		system("PAUSE");
	}

	return 0;
}