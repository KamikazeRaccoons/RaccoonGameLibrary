#include "OtherState.h"
#include <rglGame.h>
#include <rglInputHandler.h>
#include <rglDebugger.h>
#include "InitState.h"

std::string OtherState::m_stateID = "OtherState";

void OtherState::update()
{
	if (rglInputHandler::getInstance()->isKeyDown(SDL_SCANCODE_LEFT))
		rglGame::getInstance()->getGameStateMachine()->changeState(new InitState());
}

void OtherState::render()
{
	SDL_SetRenderDrawColor(rglGame::getInstance()->getRenderer(), 0, 255, 0, 255);
	SDL_RenderClear(rglGame::getInstance()->getRenderer());
}

bool OtherState::onEnter()
{
	rglDebugger::log("OtherState Entering...");
	return true;
}

bool OtherState::onExit()
{
	rglDebugger::log("OtherState Exiting...");
	return true;
}

std::string OtherState::getStateID() const
{
	return m_stateID;
}