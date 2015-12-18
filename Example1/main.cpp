#include <iostream>
#include <rglGame.h>
#include <rglDebugger.h>
#include "InitState.h"

int main(int argc, char** argv)
{
	// TODO: Thoroughly test everything.

	if (!rglGame::getInstance()->run("Example1", 640, 480, new InitState()))
	{
		rglDebugger::log("Could not initialize game.\n", rglDebugger::ERROR);
		system("PAUSE");
	}

	return 0;
}