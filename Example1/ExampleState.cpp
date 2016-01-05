#include "ExampleState.h"
#include <rglGame.h>
#include <rglStateParser.h>
#include <rglLevelParser.h>
#include <rglObjectFactory.h>
#include <rglInputHandler.h>
#include <rglTextureManager.h>
#include <rglDebugger.h>
#include <rglButton.h>

std::string ExampleState::m_stateID = "InitState";

void ExampleState::update()
{
	for (unsigned int i = 0; i < m_gameObjects.size(); i++)
		m_gameObjects[i]->update();
}

void ExampleState::render()
{
	SDL_SetRenderDrawColor(rglGame::getInstance()->getRenderer(), 127, 127, 127, 255);
	SDL_RenderClear(rglGame::getInstance()->getRenderer());

	m_pLevel->render();

	for (unsigned int i = 0; i < m_gameObjects.size(); i++)
		m_gameObjects[i]->draw();
}

void ExampleState::onEnter()
{
	rglDebugger::log("InitState Entering...");

	rglLevelParser levelParser;
	m_pLevel = levelParser.parseLevel("assets/levels/map1.tmx", "assets/textures/");

	updateCallbacks();
}

void ExampleState::onExit()
{
	rglDebugger::log("InitState Exiting...");

	for (unsigned int i = 0; i < m_gameObjects.size(); i++)
		m_gameObjects[i]->clean();

	m_gameObjects.clear();

	for (unsigned int i = 0; i < m_textureIDs.size(); i++)
		rglTextureManager::getInstance()->unload(m_textureIDs[i]);
}

std::string ExampleState::getStateID() const
{
	return m_stateID;
}