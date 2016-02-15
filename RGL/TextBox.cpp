#include "TextBox.h"
#include "Game.h"
#include "Debugger.h"
#include "InputHandler.h"
#include "FontManager.h"
#include <SDL.h>
#include <algorithm>

namespace rgl
{
	void TextBox::onCreate()
	{
		SDL_StartTextInput();
	}

	void TextBox::onDestroy()
	{
	}

	void TextBox::update()
	{
		if (InputHandler::get()->getMouseButtonState(InputHandler::LEFT))
		{
			std::shared_ptr<Vector2> pMousePos = InputHandler::get()->getMousePosition();

			if (pMousePos->getX() < (m_x + m_width) &&
				pMousePos->getX() > m_x &&
				pMousePos->getY() < (m_y + m_height) &&
				pMousePos->getY() > m_y)
				m_enabled = true;
			else
				m_enabled = false;
		}

		if (!m_enabled)
			return;

		m_text.insert(m_text.size() - m_cursorPosition, InputHandler::get()->getInputText());
		
		for (int i = 0; i < InputHandler::get()->getBackspaceCount(); i++)
		{
			if (m_text.size() != 0 && m_cursorPosition != m_text.size())
				m_text.erase(m_text.end() - m_cursorPosition - 1);
		}

		m_cursorPosition += InputHandler::get()->getRelativeCursorPosition();

		if (m_cursorPosition < 0)
			m_cursorPosition = 0;
		else if (m_cursorPosition > m_text.size())
			m_cursorPosition = m_text.size();
	}

	void TextBox::draw()
	{
		SDL_Rect backgroundRect;
		backgroundRect.x = m_x;
		backgroundRect.y = m_y;
		backgroundRect.w = m_width;
		backgroundRect.h = m_height;

		SDL_SetRenderDrawColor(Game::get()->getRenderer(), m_r, m_g, m_b, m_enabled ? m_a : m_a / 2);
		SDL_RenderFillRect(Game::get()->getRenderer(), &backgroundRect);

		int textureWidth;
		int textureHeight;
		FontManager::get()->draw(m_fontID, m_text, m_x, m_y, 0, 0, 0, m_enabled ? 255 : 127, 0, 0, 0, 0, &textureWidth, &textureHeight);

		if (!m_enabled)
			return;

		Vector2 charSize = FontManager::get()->getCharacterSize(m_fontID);

		SDL_Rect cursorRect;
		cursorRect.x = m_x + (m_text.size() - m_text.find_last_of('\n', m_text.size() - m_cursorPosition - 1) - 1 - m_cursorPosition) * (int)charSize.getX();
		cursorRect.y = m_y + std::count(m_text.begin(), m_text.end() - m_cursorPosition, '\n') * (int)charSize.getY();
		cursorRect.w = 2;
		cursorRect.h = (int)charSize.getY();

		SDL_SetRenderDrawColor(Game::get()->getRenderer(), 0, 0, 0, 255);
		SDL_RenderFillRect(Game::get()->getRenderer(), &cursorRect);
	}

	void TextBox::setText(std::string text)
	{
		m_text = text;
	}

	std::string TextBox::getText()
	{
		return m_text;
	}
}
