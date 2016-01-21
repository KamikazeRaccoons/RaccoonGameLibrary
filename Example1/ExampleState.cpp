#include "ExampleState.h"

void ExampleState::update()
{
	m_pLevel->update();
}

void ExampleState::render()
{
	SDL_SetRenderDrawColor(rgl::Game::get()->getRenderer(), 127, 127, 127, 255);
	SDL_RenderClear(rgl::Game::get()->getRenderer());

	m_pLevel->render();
}

void ExampleState::onEnter()
{
	rgl::LevelParser levelParser;
	m_pLevel = levelParser.parseLevel("assets/levels/map1/", "map1.tmx");

	m_pLevel->addCallback(quitState);
	m_pLevel->addCallback(playShoot);

	rgl::SoundManager::get()->load("assets/sounds/Gunshot.wav", "Gunshot", rgl::SoundManager::SFX);
}

void ExampleState::onExit()
{
	m_pLevel->clean();
}

std::string ExampleState::getStateID() const
{
	return "ExampleState";
}

void ExampleState::quitState()
{
	rgl::Game::get()->quit();
}

void ExampleState::playShoot()
{
	rgl::SoundManager::get()->playSound("Gunshot", 0);
}