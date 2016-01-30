#pragma once

#include <vector>
#include <memory>

#include <SDL.h>

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

		InputHandler();
		~InputHandler() { }

		void onMouseButtonDown(SDL_Event& event);
		void onMouseButtonUp(SDL_Event& event);
		void onMouseMove(SDL_Event& event);

	public:

		enum MouseButtons
		{
			LEFT = 0,
			MIDDLE = 1,
			RIGHT = 2
		};

		static InputHandler* get();

		bool getMouseButtonState(int buttonID);
		std::shared_ptr<Vector2> getMousePosition();

		bool isKeyDown(SDL_Scancode key);

		void update();

	};
}