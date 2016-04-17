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
		: GameObject(name), m_enabled(enabled), m_invalidated(false),
		m_x(x), m_y(y), m_width(width), m_height(height), m_cursorWidth(cursorWidth),
		m_fr(fr), m_fg(fg), m_fb(fb), m_fa(fa),
		m_br(br), m_bg(bg), m_bb(bb), m_ba(ba),
		m_fontID(fontID), m_textID(m_name + "_text")
	{
		m_charWidth = FontManager::get()->getCharWidth(fontID);
		m_charHeight = FontManager::get()->getCharHeight(fontID);

		m_primaryCursor.row = m_primaryCursor.column = 0;
		m_secondaryCursor = m_primaryCursor;
	}

	void TextBox::update()
	{
		m_oldText = m_text;

		if (!m_enabled)
			return;

		if (InputHandler::get()->getMouseButtonState(InputHandler::LEFT))
		{
			std::shared_ptr<Vector2> pMousePos = InputHandler::get()->getMousePosition();

			if (pMousePos->getX() < (m_x + m_width) &&
				pMousePos->getX() >= m_x &&
				pMousePos->getY() < (m_y + m_height) &&
				pMousePos->getY() >= m_y)
			{
				int numRows = getNumRows();
				m_primaryCursor.row = max(min(((int)pMousePos->getY() - m_y) / m_charHeight, numRows - 1), 0);

				int rowLength = getRowLength(m_primaryCursor.row);
				m_primaryCursor.column = max(min((int)std::round((pMousePos->getX() - (double)m_x) / (double)m_charWidth), rowLength), 0);

				std::vector<Uint8> polledButtonPresses = InputHandler::get()->getPolledButtonPresses();

				if (std::find(polledButtonPresses.begin(), polledButtonPresses.end(), SDL_BUTTON_LEFT) != polledButtonPresses.end())
					m_secondaryCursor = m_primaryCursor;
			}
		}

		std::vector<SDL_Scancode> polledKeydowns = InputHandler::get()->getPolledKeydowns();

		for (auto scancode : polledKeydowns)
		{
			switch (scancode)
			{
			case SDL_SCANCODE_DOWN:
				onDownPressed();
				break;
			case SDL_SCANCODE_UP:
				onUpPressed();
				break;
			case SDL_SCANCODE_RIGHT:
				onRightPressed();
				break;
			case SDL_SCANCODE_LEFT:
				onLeftPressed();
				break;
			case SDL_SCANCODE_DELETE:
				onDeletePressed();
				break;
			case SDL_SCANCODE_BACKSPACE:
				onBackspacePressed();
				break;
			case SDL_SCANCODE_RETURN:
				onReturnPressed();
				break;
			case SDL_SCANCODE_TAB:
				onTabPressed();
				break;
			}
		}

		std::string inputText = InputHandler::get()->getInputText();

		if (inputText.size() > 0)
		{
			if (isSelecting())
				clearSelection();

			m_text.insert(getCursorIndex(m_primaryCursor), inputText);
			m_primaryCursor.column += inputText.size();

			m_secondaryCursor = m_primaryCursor;
		}
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

		if (m_enabled)
		{
			if (isSelecting())
			{
				Cursor* pUpperCursor;
				Cursor* pLowerCursor;

				if (getCursorIndex(m_primaryCursor) < getCursorIndex(m_secondaryCursor))
				{
					pUpperCursor = &m_primaryCursor;
					pLowerCursor = &m_secondaryCursor;
				}
				else
				{
					pUpperCursor = &m_secondaryCursor;
					pLowerCursor = &m_primaryCursor;
				}

				SDL_SetRenderDrawColor(Game::get()->getRenderer(), 0, 95, 255, 127);

				for (int i = pLowerCursor->row; i >= pUpperCursor->row; i--)
				{
					int currentRowLength = getRowLength(i);

					SDL_Rect fillRect;
					fillRect.x = m_x;
					fillRect.y = m_y + i * m_charHeight;
					fillRect.w = max(currentRowLength * m_charWidth, m_cursorWidth * 2);
					fillRect.h = m_charHeight;

					if (i == pUpperCursor->row)
					{
						int offset = pUpperCursor->column * m_charWidth;

						fillRect.x += offset;
						fillRect.w -= offset;
					}

					if (i == pLowerCursor->row)
						fillRect.w -= (currentRowLength - pLowerCursor->column) * m_charWidth;

					SDL_RenderFillRect(Game::get()->getRenderer(), &fillRect);
				}
			}

			SDL_Rect primaryCursorRect;
			primaryCursorRect.x = m_x + m_primaryCursor.column * m_charWidth;
			primaryCursorRect.y = m_y + m_primaryCursor.row * m_charHeight;
			primaryCursorRect.w = m_cursorWidth;
			primaryCursorRect.h = m_charHeight;

			SDL_SetRenderDrawColor(Game::get()->getRenderer(), 0, 0, 0, 255);
			SDL_RenderFillRect(Game::get()->getRenderer(), &primaryCursorRect);
		}

		if (isTextModified() || !FontManager::get()->isTextCached(m_textID))
		{
			Debugger::get()->log("Text was modified!");
			FontManager::get()->cacheText(m_textID, m_fontID, m_text, m_fr, m_fg, m_fb, m_fa);
		}

		FontManager::get()->drawText(m_textID, m_x, m_y);

		m_invalidated = false;
	}

	void TextBox::onDestroy()
	{
		GameObject::onDestroy();
		FontManager::get()->freeText(m_textID);
	}

	void TextBox::onDownPressed()
	{
		if (m_primaryCursor.row < getNumRows() - 1)
		{
			int rowLength = getRowLength(++m_primaryCursor.row);

			if (m_primaryCursor.column > rowLength)
				m_primaryCursor.column = rowLength;
		}

		if (!InputHandler::get()->isKeyDown(SDL_SCANCODE_LSHIFT) && !InputHandler::get()->isKeyDown(SDL_SCANCODE_RSHIFT))
			m_secondaryCursor = m_primaryCursor;
	}

	void TextBox::onUpPressed()
	{
		if (m_primaryCursor.row > 0)
		{
			int rowLength = getRowLength(--m_primaryCursor.row);

			if (m_primaryCursor.column > rowLength)
				m_primaryCursor.column = rowLength;
		}

		if (!InputHandler::get()->isKeyDown(SDL_SCANCODE_LSHIFT) && !InputHandler::get()->isKeyDown(SDL_SCANCODE_RSHIFT))
			m_secondaryCursor = m_primaryCursor;
	}

	void TextBox::onRightPressed()
	{
		if (m_primaryCursor.column < getRowLength(m_primaryCursor.row))
		{
			m_primaryCursor.column++;
		}
		else if (m_primaryCursor.row < getNumRows() - 1)
		{
			m_primaryCursor.column = 0;
			m_primaryCursor.row++;
		}

		if (!InputHandler::get()->isKeyDown(SDL_SCANCODE_LSHIFT) && !InputHandler::get()->isKeyDown(SDL_SCANCODE_RSHIFT))
			m_secondaryCursor = m_primaryCursor;
	}

	void TextBox::onLeftPressed()
	{
		if (m_primaryCursor.column > 0)
			m_primaryCursor.column--;
		else if (m_primaryCursor.row > 0)
			m_primaryCursor.column = getRowLength(--m_primaryCursor.row);

		if (!InputHandler::get()->isKeyDown(SDL_SCANCODE_LSHIFT) && !InputHandler::get()->isKeyDown(SDL_SCANCODE_RSHIFT))
			m_secondaryCursor = m_primaryCursor;
	}

	void TextBox::onDeletePressed()
	{
		if (isSelecting())
		{
			clearSelection();
		}
		else
		{
			int primaryCursorIndex = getCursorIndex(m_primaryCursor);

			if (primaryCursorIndex < (int)m_text.size())
				m_text.erase(m_text.begin() + primaryCursorIndex);
		}
	}

	void TextBox::onBackspacePressed()
	{
		if (isSelecting())
		{
			clearSelection();
		}
		else
		{
			if (m_primaryCursor.column > 0)
				m_primaryCursor.column--;
			else if (m_primaryCursor.row > 0)
				m_primaryCursor.column = getRowLength(--m_primaryCursor.row);
			else
				return;

			int primaryCursorIndex = getCursorIndex(m_primaryCursor);

			if (primaryCursorIndex < (int)m_text.size())
				m_text.erase(m_text.begin() + primaryCursorIndex);

			m_secondaryCursor = m_primaryCursor;
		}
	}

	void TextBox::onReturnPressed()
	{
		if (isSelecting())
			clearSelection();

		m_text.insert(getCursorIndex(m_primaryCursor), "\n");
		m_primaryCursor.column = 0;
		m_primaryCursor.row++;

		m_secondaryCursor = m_primaryCursor;
	}

	void TextBox::onTabPressed()
	{
		if (isSelecting())
			clearSelection();

		m_text.insert(getCursorIndex(m_primaryCursor), "    ");
		m_primaryCursor.column += 4;

		m_secondaryCursor = m_primaryCursor;
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

	int TextBox::getCursorIndex(Cursor& cursor)
	{
		std::vector<int> newlineIDs = getNewlineIndices();
		return newlineIDs[cursor.row] + cursor.column;
	}

	void TextBox::clearSelection()
	{
		int primaryCursorIndex = getCursorIndex(m_primaryCursor);
		int secondaryCursorIndex = getCursorIndex(m_secondaryCursor);

		m_text.erase(m_text.begin() + min(primaryCursorIndex, secondaryCursorIndex), m_text.begin() + max(primaryCursorIndex, secondaryCursorIndex));

		if (secondaryCursorIndex < primaryCursorIndex)
			m_primaryCursor = m_secondaryCursor;
		else
			m_secondaryCursor = m_primaryCursor;
	}

	void TextBox::invalidate()
	{
		m_invalidated = true;
	}

	bool TextBox::isSelecting()
	{
		return m_secondaryCursor != m_primaryCursor;
	}

	bool TextBox::isTextModified()
	{
		return m_oldText != m_text || m_invalidated;
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
