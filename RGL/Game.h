#pragma once

#include "stdafx.h"
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
		bool m_running;

		double m_deltaTime;

		int m_width;
		int m_height;

		std::shared_ptr<GameStateMachine> m_pGameStateMachine;

		Game() : m_running(false) { }
		~Game() { }

		RGL_API void pollEvents();
		RGL_API void update();
		RGL_API void render();
		RGL_API void clean();

	public:

		static RGL_API Game* get();

		RGL_API void setFrameRate(double frameRate);
		RGL_API double getDeltaTime() const;

		RGL_API SDL_Window* getWindow();
		RGL_API SDL_Renderer* getRenderer();

		RGL_API bool isDebugEnabled() const;

		RGL_API int getWidth() const;
		RGL_API int getHeight() const;

		RGL_API std::shared_ptr<GameStateMachine> getGameStateMachine();

		RGL_API bool run(std::string title, int width, int height, std::shared_ptr<GameState> pInitState,
			bool useDebugging = false, bool fullscreen = false, double frameRate = 60.0);

		RGL_API void quit();

	};

}