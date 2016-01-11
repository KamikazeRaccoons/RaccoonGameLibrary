#pragma once

#include "stdafx.h"
#include "Vector2.h"

#ifdef RGL_EXPORTS
#define RGL_INPUTHANDLER_API __declspec(dllexport)
#else
#define RGL_INPUTHANDLER_API __declspec(dllimport)
#endif

namespace rgl
{
	class InputHandler
	{
	private:

		static InputHandler* m_pInstance;

		std::vector<bool> m_mouseButtonStates;

		std::shared_ptr<Vector2> m_pMousePosition;

		const Uint8* m_pKeystates;

		RGL_INPUTHANDLER_API InputHandler();
		~InputHandler() { }

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

		static RGL_INPUTHANDLER_API InputHandler* getInstance();

		RGL_INPUTHANDLER_API bool getMouseButtonState(int buttonID);
		RGL_INPUTHANDLER_API std::shared_ptr<Vector2> getMousePosition();

		RGL_INPUTHANDLER_API bool isKeyDown(SDL_Scancode key);

		RGL_INPUTHANDLER_API void update();

	};
}