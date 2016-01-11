#pragma once

#include "stdafx.h"
#include "GameState.h"
#include "GameStateMachine.h"

#ifdef RGL_EXPORTS
#define RGL_GAME_API __declspec(dllexport)
#else
#define RGL_GAME_API __declspec(dllimport)
#endif

namespace rgl
{
	class Game
	{
	private:

		static Game* m_pInstance;

		SDL_Window* m_pWindow;
		SDL_Renderer* m_pRenderer;

		bool m_running;

		double m_deltaTime;

		int m_width;
		int m_height;

		std::shared_ptr<GameStateMachine> m_pGameStateMachine;

		Game() : m_running(false) { }
		~Game() { }

		RGL_GAME_API void pollEvents();
		RGL_GAME_API void update();
		RGL_GAME_API void render();
		RGL_GAME_API void clean();

	public:

		static RGL_GAME_API Game* getInstance();

		RGL_GAME_API void setFrameRate(double frameRate);
		RGL_GAME_API double getDeltaTime() const;

		RGL_GAME_API SDL_Window* getWindow();
		RGL_GAME_API SDL_Renderer* getRenderer();

		RGL_GAME_API int getWidth() const;
		RGL_GAME_API int getHeight() const;

		RGL_GAME_API std::shared_ptr<GameStateMachine> getGameStateMachine();

		RGL_GAME_API bool run(std::string title, int width, int height, std::shared_ptr<GameState> pInitState, bool fullscreen = false,
			double frameRate = 60.0);

		RGL_GAME_API void quit();

	};

}