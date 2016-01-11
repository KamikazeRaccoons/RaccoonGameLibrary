#include <iostream>

#include <RGL.h>
#include "ExampleState.h"
#include "Helicopter.h"

int main(int argc, char** argv)
{
	rgl::ObjectFactory::getInstance()->registerType("Helicopter", std::make_shared<HelicopterCreator>());

	if (!rgl::Game::getInstance()->run("Example1", 640, 480, std::make_shared<ExampleState>()))
	{
		rgl::Debugger::log("Could not initialize game.\n", rgl::Debugger::ERROR);
		system("PAUSE");
	}

	return 0;
}