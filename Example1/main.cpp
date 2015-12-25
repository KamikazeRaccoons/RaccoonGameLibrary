#include <iostream>
#include <rglGame.h>
#include <rglDebugger.h>
#include "InitState.h"
#include "OtherState.h"

int main(int argc, char** argv)
{
	// TODO: Code looks good. Push changes.

	if (!rglGame::getInstance()->run("Example1", 640, 480, make_shared<InitState>()))
	{
		rglDebugger::log("Could not initialize game.\n", rglDebugger::ERROR);
		system("PAUSE");
	}

	return 0;
}