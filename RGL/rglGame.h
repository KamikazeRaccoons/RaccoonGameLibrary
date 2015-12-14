#pragma once

#include "stdafx.h"

#ifdef RGL_EXPORTS
#define RGL_GAME_API __declspec(dllexport)
#else
#define RGL_GAME_API __declspec(dllimport)
#endif

class rglGame
{
private:

	static rglGame* m_pInstance;

	bool m_running;

	double m_frameRate;
	
	SDL_Window* m_pWindow;
	SDL_Renderer* m_pRenderer;

	rglGame() : m_running(false) { }
	~rglGame() { }

	RGL_GAME_API void pollEvents();
	RGL_GAME_API void update();
	RGL_GAME_API void render();
	RGL_GAME_API void clean();

public:

	static RGL_GAME_API rglGame* getInstance();

	RGL_GAME_API void setFrameRate(double frameRate);
	RGL_GAME_API double getFrameRate();

	RGL_GAME_API SDL_Renderer* getRenderer();

	RGL_GAME_API bool run(std::string title, int width, int height, bool fullscreen = false,
		double frameRate = 60.0);

	// TODO: Implement the input handler.

	RGL_GAME_API void quit();

};

