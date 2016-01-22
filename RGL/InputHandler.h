#pragma once

#include "stdafx.h"
#include "Vector2.h"

namespace rgl
{
	class InputHandler
	{
	private:

		static InputHandler* m_pInstance;

		std::vector<bool> m_mouseButtonStates;

		std::shared_ptr<Vector2> m_pMousePosition;

		const Uint8* m_pKeystates;

		RGL_API InputHandler();
		~InputHandler() { }

		RGL_API void onMouseButtonDown(SDL_Event& event);
		RGL_API void onMouseButtonUp(SDL_Event& event);
		RGL_API void onMouseMove(SDL_Event& event);

	public:

		enum MouseButtons
		{
			LEFT = 0,
			MIDDLE = 1,
			RIGHT = 2
		};

		static RGL_API InputHandler* get();

		RGL_API bool getMouseButtonState(int buttonID);
		RGL_API std::shared_ptr<Vector2> getMousePosition();

		RGL_API bool isKeyDown(SDL_Scancode key);

		RGL_API void update();

	};
}