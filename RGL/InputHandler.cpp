#include "InputHandler.h"
#include "Debugger.h"
#include "Game.h"

namespace rgl
{
	InputHandler* InputHandler::m_pInstance = 0;

	InputHandler::InputHandler()
	{
		for (int i = 0; i < 3; i++)
			m_mouseButtonStates.push_back(false);

		m_pMousePosition = std::make_shared<Vector2>();
	}

	InputHandler* InputHandler::get()
	{
		if (m_pInstance == 0)
			m_pInstance = new InputHandler();

		return m_pInstance;
	}

	void InputHandler::onMouseButtonDown(SDL_Event& event)
	{
		switch (event.button.button)
		{
		case SDL_BUTTON_LEFT:
			m_mouseButtonStates[LEFT] = true;
			break;
		case SDL_BUTTON_MIDDLE:
			m_mouseButtonStates[MIDDLE] = true;
			break;
		case SDL_BUTTON_RIGHT:
			m_mouseButtonStates[RIGHT] = true;
			break;
		}
	}

	void InputHandler::onMouseButtonUp(SDL_Event& event)
	{
		switch (event.button.button)
		{
		case SDL_BUTTON_LEFT:
			m_mouseButtonStates[LEFT] = false;
			break;
		case SDL_BUTTON_MIDDLE:
			m_mouseButtonStates[MIDDLE] = false;
			break;
		case SDL_BUTTON_RIGHT:
			m_mouseButtonStates[RIGHT] = false;
			break;
		}
	}

	void InputHandler::onMouseMove(SDL_Event& event)
	{
		m_pMousePosition->setX(event.motion.x);
		m_pMousePosition->setY(event.motion.y);
	}

	bool InputHandler::getMouseButtonState(int buttonID)
	{
		return m_mouseButtonStates[buttonID];
	}

	std::shared_ptr<Vector2> InputHandler::getMousePosition()
	{
		return m_pMousePosition;
	}

	bool InputHandler::isKeyDown(SDL_Scancode key)
	{
		if (m_pKeystates == 0)
			return false;

		return m_pKeystates[key] == 1 ? true : false;
	}

	std::string InputHandler::getInputText()
	{
		return m_inputText;
	}

	int InputHandler::getBackspaceCount()
	{
		return m_backspaceCount;
	}

	int InputHandler::getRelativeCursorPosition()
	{
		return m_relativeCursorPosition;
	}

	void InputHandler::update()
	{
		m_pKeystates = SDL_GetKeyboardState(0);

		m_inputText.clear();
		m_backspaceCount = 0;
		m_relativeCursorPosition = 0;

		SDL_Event event;

		SDL_StartTextInput();

		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				Game::get()->quit();
				break;
			case SDL_MOUSEBUTTONDOWN:
				onMouseButtonDown(event);
				break;
			case SDL_MOUSEBUTTONUP:
				onMouseButtonUp(event);
				break;
			case SDL_MOUSEMOTION:
				onMouseMove(event);
				break;
			case SDL_TEXTINPUT:
				m_inputText += event.text.text;
				break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.scancode)
				{
				case SDL_SCANCODE_BACKSPACE:
					if (m_inputText.size() == 0)
						m_backspaceCount++;
					else
						m_inputText.pop_back();
					break;
				case SDL_SCANCODE_RETURN:
					m_inputText += '\n';
					break;
				case SDL_SCANCODE_TAB:
					m_inputText += "    ";
					break;
				case SDL_SCANCODE_LEFT:
					m_relativeCursorPosition++;
					break;
				case SDL_SCANCODE_RIGHT:
					m_relativeCursorPosition--;
					break;
				}
				break;
			}
		}

		SDL_StopTextInput();
	}
}