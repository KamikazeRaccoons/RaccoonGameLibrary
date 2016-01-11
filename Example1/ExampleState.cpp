#include "ExampleState.h"
#include <Game.h>
#include <StateParser.h>
#include <LevelParser.h>
#include <ObjectFactory.h>
#include <InputHandler.h>
#include <TextureManager.h>
#include <Debugger.h>
#include <Button.h>

std::string ExampleState::m_stateID = "InitState";

void ExampleState::update()
{
	m_pLevel->update();

	for (unsigned int i = 0; i < m_gameObjects.size(); i++)
		m_gameObjects[i]->update();
}

void ExampleState::render()
{
	SDL_SetRenderDrawColor(rgl::Game::getInstance()->getRenderer(), 127, 127, 127, 255);
	SDL_RenderClear(rgl::Game::getInstance()->getRenderer());

	m_pLevel->render();

	for (unsigned int i = 0; i < m_gameObjects.size(); i++)
		m_gameObjects[i]->draw();
}

void ExampleState::onEnter()
{
	rgl::Debugger::log("InitState Entering...");

	rgl::LevelParser levelParser;
	m_pLevel = levelParser.parseLevel("assets/levels/", "map1.tmx");

	updateCallbacks();
}

void ExampleState::onExit()
{
	rgl::Debugger::log("InitState Exiting...");

	for (unsigned int i = 0; i < m_gameObjects.size(); i++)
		m_gameObjects[i]->clean();

	m_gameObjects.clear();

	for (unsigned int i = 0; i < m_textureIDs.size(); i++)
		rgl::TextureManager::getInstance()->unload(m_textureIDs[i]);
}

std::string ExampleState::getStateID() const
{
	return m_stateID;
}