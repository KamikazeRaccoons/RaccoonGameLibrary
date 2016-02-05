#include <iostream>

#include <RGL.h>

#include "ExampleState.h"
#include "Crate.h"

int main(int argc, char** argv)
{
	rgl::ObjectFactory::get()->registerType("Crate", std::make_shared<CrateCreator>());

	if (!rgl::Game::get()->run("Example1", 640, 480, std::make_shared<ExampleState>(), false))
	{
		rgl::Debugger::get()->log("Could not initialize game.\n", rgl::Debugger::FATAL_ERROR);
		system("PAUSE");
	}

	return 0;
}