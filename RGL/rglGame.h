#pragma once

#include "stdafx.h"
#include "rglGameState.h"
#include "rglGameStateMachine.h"

#ifdef RGL_EXPORTS
#define RGL_GAME_API __declspec(dllexport)
#else
#define RGL_GAME_API __declspec(dllimport)
#endif

class rglGame
{
private:

	static rglGame* m_pInstance;
	
	SDL_Window* m_pWindow;
	SDL_Renderer* m_pRenderer;

	bool m_running;

	double m_deltaTime;

	int m_width;
	int m_height;

	shared_ptr<rglGameStateMachine> m_pGameStateMachine;

	rglGame() : m_running(false) { }
	~rglGame() { }

	RGL_GAME_API void pollEvents();
	RGL_GAME_API void update();
	RGL_GAME_API void render();
	RGL_GAME_API void clean();

public:

	static RGL_GAME_API rglGame* getInstance();

	RGL_GAME_API void setFrameRate(double frameRate);
	RGL_GAME_API double getDeltaTime() const;

	RGL_GAME_API SDL_Window* getWindow();
	RGL_GAME_API SDL_Renderer* getRenderer();

	RGL_GAME_API int getWidth() const;
	RGL_GAME_API int getHeight() const;

	RGL_GAME_API shared_ptr<rglGameStateMachine> getGameStateMachine();

	RGL_GAME_API bool run(string title, int width, int height, shared_ptr<rglGameState> pInitState, bool fullscreen = false,
		double frameRate = 60.0);

	RGL_GAME_API void quit();

};

