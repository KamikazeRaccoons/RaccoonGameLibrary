#include "ExampleState.h"

void ExampleState::update()
{
	if (rgl::InputHandler::get()->isKeyDown(SDL_SCANCODE_LEFT))
		m_pLevel->getVelocity().setX(-10.0f);
	else if (rgl::InputHandler::get()->isKeyDown(SDL_SCANCODE_RIGHT))
		m_pLevel->getVelocity().setX(10.0f);
	else
		m_pLevel->getVelocity() *= 0.75f;

	m_pLevel->update();
}

void ExampleState::render()
{
	SDL_SetRenderDrawColor(rgl::Game::get()->getRenderer(), 0, 191, 255, 255);
	SDL_RenderClear(rgl::Game::get()->getRenderer());

	m_pLevel->render();
}

void ExampleState::onEnter()
{
	m_pLevel = rgl::LevelParser::parseLevel("assets/levels/map1/", "map1.tmx");

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