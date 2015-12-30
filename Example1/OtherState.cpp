#include "OtherState.h"
#include <rglGame.h>
#include <rglInputHandler.h>
#include <rglTextureManager.h>
#include <rglButton.h>
#include <rglDebugger.h>
#include "InitState.h"

std::string OtherState::m_stateID = "OtherState";

void OtherState::update()
{
	for (unsigned int i = 0; i < m_gameObjects.size(); i++)
		m_gameObjects[i]->update();
}

void OtherState::render()
{
	SDL_SetRenderDrawColor(rglGame::getInstance()->getRenderer(), 0, 255, 0, 255);
	SDL_RenderClear(rglGame::getInstance()->getRenderer());

	for (unsigned int i = 0; i < m_gameObjects.size(); i++)
		m_gameObjects[i]->draw();
}

void OtherState::onEnter()
{
	rglDebugger::log("OtherState Entering...");

	SDL_SetWindowTitle(rglGame::getInstance()->getWindow(), "OtherState");

	if (!rglTextureManager::getInstance()->load("assets/textures/InitState.png", "InitStateButton"))
	{
		rglDebugger::log("Could not load \"InitState.png\"", rglDebugger::ERROR);
		return;
	}

	int w, h;
	SDL_GetWindowSize(rglGame::getInstance()->getWindow(), &w, &h);

	m_gameObjects.push_back(make_shared<rglButton>(make_shared<rglObjectParams>(w / 2 - 32, h / 2 - 16, 64, 32, "InitStateButton"), &onInitStateClick));
}

void OtherState::onExit()
{
	rglDebugger::log("OtherState Exiting...");

	for (unsigned int i = 0; i < m_gameObjects.size(); i++)
		m_gameObjects[i]->clean();

	m_gameObjects.clear();

	rglTextureManager::getInstance()->unload("InitStateButton");
}

std::string OtherState::getStateID() const
{
	return m_stateID;
}

void OtherState::onInitStateClick()
{
	rglGame::getInstance()->getGameStateMachine()->changeState(make_shared<InitState>());
}