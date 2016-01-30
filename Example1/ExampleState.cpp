#include "ExampleState.h"
#include "Crate.h"

void ExampleState::update()
{
	if (rgl::InputHandler::get()->isKeyDown(SDL_SCANCODE_RETURN))
		rgl::Game::get()->getGameStateMachine()->changeState(std::make_shared<ExampleState>());
	if (rgl::InputHandler::get()->isKeyDown(SDL_SCANCODE_LEFT))
		m_pLevel->getVelocity().setX(-10.0f);
	else if (rgl::InputHandler::get()->isKeyDown(SDL_SCANCODE_RIGHT))
		m_pLevel->getVelocity().setX(10.0f);
	else
		m_pLevel->getVelocity() *= 0.75f;

	if (rgl::InputHandler::get()->getMouseButtonState(rgl::InputHandler::MIDDLE))
	{
		spawnCrate();
	}
	else if (rgl::InputHandler::get()->getMouseButtonState(rgl::InputHandler::LEFT))
	{
		if (!m_pressed)
		{
			m_pressed = true;
			spawnCrate();
		}
	}
	else
	{
		m_pressed = false;
	}

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

	rgl::FontManager::get()->load("assets/fonts/segoeuil.ttf", 18, "Segoe");
}

void ExampleState::onExit()
{
	m_pLevel->clean();

	rgl::FontManager::get()->clear();
}

std::string ExampleState::getStateID() const
{
	return "ExampleState";
}

void ExampleState::spawnCrate()
{
	m_pLevel->addObject(std::make_shared<Crate>(
		(int)(rgl::InputHandler::get()->getMousePosition()->getX() + m_pLevel->getPosition().getX()),
		(int)(rgl::InputHandler::get()->getMousePosition()->getY() + m_pLevel->getPosition().getY()),
		64, 64, "Crate", "Crate" + std::to_string(m_crateID)), 0);
	m_crateID++;
}

void ExampleState::quitState()
{
	rgl::Game::get()->quit();
}