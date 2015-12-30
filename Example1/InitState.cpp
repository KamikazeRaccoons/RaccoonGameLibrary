#include "InitState.h"
#include <rglGame.h>
#include <rglInputHandler.h>
#include <rglTextureManager.h>
#include <rglDebugger.h>
#include <rglButton.h>
#include "OtherState.h"

std::string InitState::m_stateID = "InitState";

void InitState::update()
{
	for (unsigned int i = 0; i < m_gameObjects.size(); i++)
		m_gameObjects[i]->update();
}

void InitState::render()
{
	SDL_SetRenderDrawColor(rglGame::getInstance()->getRenderer(), 255, 0, 0, 255);
	SDL_RenderClear(rglGame::getInstance()->getRenderer());

	for (unsigned int i = 0; i < m_gameObjects.size(); i++)
		m_gameObjects[i]->draw();
}

void InitState::onEnter()
{
	rglDebugger::log("InitState Entering...");

	SDL_SetWindowTitle(rglGame::getInstance()->getWindow(), "InitState");

	if (!rglTextureManager::getInstance()->load("assets/textures/OtherState.png", "OtherStateButton"))
	{
		rglDebugger::log("Could not load \"OtherState.png\"", rglDebugger::ERROR);
		rglGame::getInstance()->getGameStateMachine()->popState();
		return;
	}

	int w, h;
	SDL_GetWindowSize(rglGame::getInstance()->getWindow(), &w, &h);

	m_gameObjects.push_back(make_shared<rglButton>(make_shared<rglObjectParams>(w / 2 - 32, h / 2 - 16, 64, 32, "OtherStateButton"), &onOtherStateClick));
}

void InitState::onExit()
{
	rglDebugger::log("InitState Exiting...");

	for (unsigned int i = 0; i < m_gameObjects.size(); i++)
		m_gameObjects[i]->clean();

	m_gameObjects.clear();

	rglTextureManager::getInstance()->unload("OtherStateButton");
}

std::string InitState::getStateID() const
{
	return m_stateID;
}

void InitState::onOtherStateClick()
{
	rglGame::getInstance()->getGameStateMachine()->changeState(make_shared<OtherState>());
}