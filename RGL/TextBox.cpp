#include "TextBox.h"
#include "Game.h"
#include "Debugger.h"
#include "InputHandler.h"
#include "FontManager.h"
#include <SDL.h>
#include <algorithm>

namespace rgl
{
	TextBox::TextBox(bool enabled, int x, int y, int width, int height, int cursorWidth,
		int fr, int fg, int fb, int fa,
		int br, int bg, int bb, int ba,
		std::string fontID, std::string name)
		: GameObject(name), m_enabled(enabled), m_x(x), m_y(y), m_width(width), m_height(height), m_cursorWidth(cursorWidth),
		m_fr(fr), m_fg(fg), m_fb(fb), m_fa(fa),
		m_br(br), m_bg(bg), m_bb(bb), m_ba(ba),
		m_fontID(fontID), m_currentRow(0), m_currentColumn(0)
	{
		Vector2 charSize = FontManager::get()->getCharacterSize(fontID);
		m_charWidth = (int)charSize.getX();
		m_charHeight = (int)charSize.getY();
	}

	void TextBox::onCreate()
	{
		SDL_StartTextInput();
	}

	void TextBox::onDestroy()
	{
	}

	void TextBox::update()
	{
		if (!m_enabled)
			return;

		if (InputHandler::get()->getMouseButtonState(InputHandler::LEFT))
		{
			std::shared_ptr<Vector2> pMousePos = InputHandler::get()->getMousePosition();

			if (pMousePos->getX() < (m_x + m_width) &&
				pMousePos->getX() > m_x &&
				pMousePos->getY() < (m_y + m_height) &&
				pMousePos->getY() > m_y)
			{
				int numRows = getNumRows();
				m_currentRow = max(min(((int)pMousePos->getY() - m_y) / m_charHeight, numRows - 1), 0);

				int rowLength = getRowLength(m_currentRow);
				m_currentColumn = max(min((int)std::round((pMousePos->getX() - (double)m_x) / (double)m_charWidth), rowLength), 0);
			}
		}

		for (auto scancode : InputHandler::get()->getPolledKeydowns())
		{
			switch (scancode)
			{
			case SDL_SCANCODE_DOWN:
				if (m_currentRow < getNumRows() - 1)
				{
					int rowLength = getRowLength(++m_currentRow);

					if (m_currentColumn > rowLength)
						m_currentColumn = rowLength;
				}
				break;
			case SDL_SCANCODE_UP:
				if (m_currentRow > 0)
				{
					int rowLength = getRowLength(--m_currentRow);

					if (m_currentColumn > rowLength)
						m_currentColumn = rowLength;
				}
				break;
			case SDL_SCANCODE_RIGHT:
				if (m_currentColumn < getRowLength(m_currentRow))
				{
					m_currentColumn++;
				}
				else if (m_currentRow < getNumRows() - 1)
				{
					m_currentColumn = 0;
					m_currentRow++;
				}
				break;
			case SDL_SCANCODE_LEFT:
				if (m_currentColumn > 0)
					m_currentColumn--;
				else if (m_currentRow > 0)
					m_currentColumn = getRowLength(--m_currentRow);
				break;
			case SDL_SCANCODE_DELETE:
			{
				int cursorIndex = getCursorIndex();

				if (cursorIndex < (int)m_text.size())
					m_text.erase(m_text.begin() + cursorIndex);
			}
				break;
			case SDL_SCANCODE_BACKSPACE:
			{
				if (m_currentColumn > 0)
					m_currentColumn--;
				else if (m_currentRow > 0)
					m_currentColumn = getRowLength(--m_currentRow);
				else
					break;

				int cursorIndex = getCursorIndex();

				if (cursorIndex < (int)m_text.size())
					m_text.erase(m_text.begin() + cursorIndex);
			}
				break;
			case SDL_SCANCODE_RETURN:
				m_text.insert(getCursorIndex(), "\n");
				m_currentColumn = 0;
				m_currentRow++;
				break;
			case SDL_SCANCODE_TAB:
				m_text.insert(getCursorIndex(), "    ");
				m_currentColumn += 4;
				break;
			}
		}

		std::string inputText = InputHandler::get()->getInputText();
		m_text.insert(getCursorIndex(), inputText);
		m_currentColumn += inputText.size();
	}

	void TextBox::draw()
	{
		SDL_Rect backgroundRect;
		backgroundRect.x = m_x;
		backgroundRect.y = m_y;
		backgroundRect.w = m_width;
		backgroundRect.h = m_height;

		SDL_SetRenderDrawColor(Game::get()->getRenderer(), m_br, m_bg, m_bb, m_ba);
		SDL_RenderFillRect(Game::get()->getRenderer(), &backgroundRect);

		FontManager::get()->draw(m_fontID, m_text, m_x, m_y, m_fr, m_fg, m_fb, m_fa, 0, 0, 0, 0);

		if (!m_enabled)
			return;

		SDL_Rect cursorRect;
		cursorRect.x = m_x + m_currentColumn * m_charWidth;
		cursorRect.y = m_y + m_currentRow * m_charHeight;
		cursorRect.w = m_cursorWidth;
		cursorRect.h = m_charHeight;

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

	std::vector<int> TextBox::getNewlineIndices()
	{
		std::vector<int> newlineIDs;
		newlineIDs.push_back(0);

		for (int i = 0; i < (int)m_text.size(); i++)
		{
			if (m_text[i] == '\n')
				newlineIDs.push_back(i + 1);
		}

		newlineIDs.push_back(m_text.size() + 1);

		return newlineIDs;
	}

	int TextBox::getNumRows()
	{
		return std::count(m_text.begin(), m_text.end(), '\n') + 1;
	}

	int TextBox::getRowLength(int row)
	{
		std::vector<int> newlineIDs = getNewlineIndices();
		return row < (int)newlineIDs.size() - 1 ? newlineIDs[row + 1] - (newlineIDs[row] + 1) : 0;
	}

	int TextBox::getCursorIndex()
	{
		std::vector<int> newlineIDs = getNewlineIndices();
		return newlineIDs[m_currentRow] + m_currentColumn;
	}
}
