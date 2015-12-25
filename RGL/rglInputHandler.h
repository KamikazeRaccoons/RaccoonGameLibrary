#pragma once

#include "stdafx.h"
#include "rglVector2.h"

#ifdef RGL_EXPORTS
#define RGL_INPUTHANDLER_API __declspec(dllexport)
#else
#define RGL_INPUTHANDLER_API __declspec(dllimport)
#endif

class rglInputHandler
{
private:

	static rglInputHandler* m_pInstance;

	std::vector<bool> m_mouseButtonStates;

	shared_ptr<rglVector2> m_pMousePosition;

	const Uint8* m_pKeystates;

	RGL_INPUTHANDLER_API rglInputHandler();
	~rglInputHandler() { }

	RGL_INPUTHANDLER_API void onMouseButtonDown(SDL_Event& event);
	RGL_INPUTHANDLER_API void onMouseButtonUp(SDL_Event& event);
	RGL_INPUTHANDLER_API void onMouseMove(SDL_Event& event);

public:

	enum MouseButtons
	{
		LEFT = 0,
		MIDDLE = 1,
		RIGHT = 2
	};

	static RGL_INPUTHANDLER_API rglInputHandler* getInstance();

	RGL_INPUTHANDLER_API bool getMouseButtonState(int buttonID);
	RGL_INPUTHANDLER_API shared_ptr<rglVector2> getMousePosition();

	RGL_INPUTHANDLER_API bool isKeyDown(SDL_Scancode key);

	RGL_INPUTHANDLER_API void update();
	RGL_INPUTHANDLER_API void clean();

};