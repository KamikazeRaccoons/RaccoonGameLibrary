#include "InitState.h"
#include <rglGame.h>
#include <rglInputHandler.h>
#include <rglDebugger.h>
#include "OtherState.h"

std::string InitState::m_stateID = "InitState";

void InitState::update()
{
	if (rglInputHandler::getInstance()->isKeyDown(SDL_SCANCODE_RIGHT))
		rglGame::getInstance()->getGameStateMachine()->changeState(new OtherState());
}

void InitState::render()
{
	SDL_SetRenderDrawColor(rglGame::getInstance()->getRenderer(), 255, 0, 0, 255);
	SDL_RenderClear(rglGame::getInstance()->getRenderer());
}

bool InitState::onEnter()
{
	rglDebugger::log("InitState Entering...");
	return true;
}

bool InitState::onExit()
{
	rglDebugger::log("InitState Exiting...");
	return true;
}

std::string InitState::getStateID() const
{
	return m_stateID;
}