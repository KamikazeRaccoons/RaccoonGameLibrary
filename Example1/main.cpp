#include <iostream>

#include <RGL.h>
#include "ExampleState.h"

int main(int argc, char** argv)
{
	if (!rgl::Game::get()->run("Example1", 640, 480, std::make_shared<ExampleState>(), true))
	{
		rgl::Debugger::get()->log("Could not initialize game.\n", rgl::Debugger::FATAL_ERROR);
		system("PAUSE");
	}

	return 0;
}