#pragma once

#include <memory>

#include <SDL.h>

#include "GameState.h"
#include "GameStateMachine.h"

namespace rgl
{
	class Game
	{
	private:

		static Game* m_pInstance;

		SDL_Window* m_pWindow;
		SDL_Renderer* m_pRenderer;

		bool m_useDebugging;
		bool m_initialized;
		bool m_running;

		double m_deltaTime;

		int m_width;
		int m_height;

		std::shared_ptr<GameStateMachine> m_pGameStateMachine;

		Game() : m_initialized(false), m_running(false) { }
		~Game() { }

		void pollEvents();
		void update();
		void render();
		void clean();

	public:

		static Game* get();

		void setFrameRate(double frameRate);
		double getDeltaTime() const;

		SDL_Window* getWindow();
		SDL_Renderer* getRenderer();

		bool isDebugEnabled() const;

		int getWidth() const;
		int getHeight() const;

		std::shared_ptr<GameStateMachine> getGameStateMachine();

		bool init(std::string title, int width, int height,
			bool useDebugging = false, bool fullscreen = false, double frameRate = 60.0);

		void run(std::shared_ptr<GameState> pInitState);

		void quit();

	};

}