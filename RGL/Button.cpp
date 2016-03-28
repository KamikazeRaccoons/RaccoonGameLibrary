#include "Button.h"
#include "Level.h"
#include "InputHandler.h"
#include "TextureManager.h"

namespace rgl
{
	void Button::onCreate()
	{
		GameObject::onCreate();

		m_callback = m_pLevel->getCallback(m_callbackID);
	}

	void Button::update()
	{
		std::shared_ptr<Vector2> pMousePos = InputHandler::get()->getMousePosition();

		if (pMousePos->getX() < (m_position.getX() + m_width) &&
			pMousePos->getX() > m_position.getX() &&
			pMousePos->getY() < (m_position.getY() + m_height) &&
			pMousePos->getY() > m_position.getY())
		{
			if (InputHandler::get()->getMouseButtonState(InputHandler::LEFT))
			{
				m_buttonState = MOUSE_PRESSED;
			}
			else
			{
				if (m_buttonState == MOUSE_PRESSED)
					m_callback();

				m_buttonState = MOUSE_HOVERING;
			}
		}
		else
		{
			m_buttonState = MOUSE_AWAY;
		}
	}

	void Button::draw()
	{
		TextureManager::get()->drawFrame(m_textureID, (int)m_position.getX(), (int)m_position.getY(), m_width, m_height, 0, m_buttonState);
	}

	int Button::getX()
	{
		return (int)(m_position.getX() + m_pLevel->getPosition().getX());
	}

	int Button::getY()
	{
		return (int)(m_position.getY() + m_pLevel->getPosition().getY());
	}

	int Button::getWidth()
	{
		return m_width;
	}

	int Button::getHeight()
	{
		return m_height;
	}
}