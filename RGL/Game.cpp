#include "Game.h"
#include "SoundManager.h"
#include "InputHandler.h"
#include "Debugger.h"
#include "FontManager.h"
#include "ObjectFactory.h"
#include "Button.h"
#include "PhysicsObject.h"

#include <chrono>

namespace rgl
{
	Game* Game::m_pInstance = 0;

	Game* Game::get()
	{
		if (m_pInstance == 0)
			m_pInstance = new Game();

		return m_pInstance;
	}

	SDL_Window* Game::getWindow()
	{
		return m_pWindow;
	}

	SDL_Renderer* Game::getRenderer()
	{
		return m_pRenderer;
	}

	bool Game::isDebugEnabled() const
	{
		return m_useDebugging;
	}

	std::shared_ptr<GameStateMachine> Game::getGameStateMachine()
	{
		return m_pGameStateMachine;
	}

	void Game::setFrameRate(double frameRate)
	{
		m_deltaTime = 1.0 / frameRate;
	}

	double Game::getDeltaTime() const
	{
		return m_deltaTime;
	}

	int Game::getWidth() const
	{
		return m_width;
	}

	int Game::getHeight() const
	{
		return m_height;
	}

	bool Game::init(std::string title, int width, int height,
		bool useDebugging, bool fullscreen, double frameRate)
	{
		if (m_initialized || m_running)
			return false;

		srand((unsigned int)std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count());

		m_useDebugging = useDebugging;

		Debugger::get()->init(title);

		if (SDL_Init(SDL_INIT_EVERYTHING) < 0 ||
			(m_pWindow = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height,
				fullscreen ? SDL_WINDOW_FULLSCREEN : SDL_WINDOW_SHOWN)) == 0 ||
			(m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC)) == 0)
			return false;

		SDL_SetRenderDrawBlendMode(m_pRenderer, SDL_BLENDMODE_BLEND);

		SoundManager::get()->init();
		FontManager::get()->init();

		m_width = width;
		m_height = height;
		m_deltaTime = 1.0 / frameRate;

		ObjectFactory::get()->registerType("Button", std::make_shared<ButtonCreator>());

		m_pGameStateMachine = std::make_shared<GameStateMachine>();

		m_initialized = true;

		return true;
	}

	void Game::run(std::shared_ptr<GameState> pInitState)
	{
		if (!m_initialized || m_running)
			return;

		if (pInitState == 0)
		{
			rgl::Debugger::get()->log("Supplied state was null.", rgl::Debugger::MESSAGE);
			return;
		}

		m_running = true;

		m_pGameStateMachine->changeState(pInitState);

		double updateTime = SDL_GetTicks() * 0.001;
		double accumulatedTime = 0.0;

		while (m_running)
		{
			double currentTime = SDL_GetTicks() * 0.001;
			double frameTime = currentTime - updateTime;
			updateTime = currentTime;
			accumulatedTime += frameTime;

			while (accumulatedTime > m_deltaTime)
			{
				pollEvents();
				update();
				accumulatedTime -= m_deltaTime;
			}

			render();
		}

		clean();
	}

	void Game::quit()
	{
		m_running = false;
	}

	void Game::pollEvents()
	{
		InputHandler::get()->update();
	}

	void Game::update()
	{
		m_pGameStateMachine->update();
	}

	void Game::render()
	{
		SDL_SetRenderDrawColor(m_pRenderer, 255, 255, 255, 255);
		SDL_RenderClear(m_pRenderer);

		m_pGameStateMachine->render();

		SDL_RenderPresent(m_pRenderer);
	}

	void Game::clean()
	{
		m_pGameStateMachine->clean();
		SoundManager::get()->clean();

		SDL_DestroyRenderer(m_pRenderer);
		SDL_DestroyWindow(m_pWindow);
		SDL_Quit();
	}
}