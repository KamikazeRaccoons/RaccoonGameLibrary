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
		m_fontID(fontID), m_primaryRow(0), m_primaryColumn(0), m_secondaryRow(0), m_secondaryColumn(0)
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
				pMousePos->getX() >= m_x &&
				pMousePos->getY() < (m_y + m_height) &&
				pMousePos->getY() >= m_y)
			{
				int numRows = getNumRows();
				m_primaryRow = max(min(((int)pMousePos->getY() - m_y) / m_charHeight, numRows - 1), 0);

				int rowLength = getRowLength(m_primaryRow);
				m_primaryColumn = max(min((int)std::round((pMousePos->getX() - (double)m_x) / (double)m_charWidth), rowLength), 0);

				std::vector<Uint8> polledButtonPresses = InputHandler::get()->getPolledButtonPresses();

				if (std::find(polledButtonPresses.begin(), polledButtonPresses.end(), SDL_BUTTON_LEFT) != polledButtonPresses.end())
					resetSecondaryCursor();
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

			m_text.insert(getCursorIndex(m_primaryRow, m_primaryColumn), inputText);
			m_primaryColumn += inputText.size();

			resetSecondaryCursor();
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
				int upperRow;
				int upperColumn;
				int lowerRow;
				int lowerColumn;

				if (getCursorIndex(m_primaryRow, m_primaryColumn) < getCursorIndex(m_secondaryRow, m_secondaryColumn))
				{
					upperRow = m_primaryRow;
					upperColumn = m_primaryColumn;
					lowerRow = m_secondaryRow;
					lowerColumn = m_secondaryColumn;
				}
				else
				{
					upperRow = m_secondaryRow;
					upperColumn = m_secondaryColumn;
					lowerRow = m_primaryRow;
					lowerColumn = m_primaryColumn;
				}

				SDL_SetRenderDrawColor(Game::get()->getRenderer(), 0, 0, 255, 127);

				for (int i = lowerRow; i >= upperRow && i >= 0; i--)
				{
					int currentRowLength = getRowLength(i);

					SDL_Rect fillRect;
					fillRect.x = m_x;
					fillRect.y = m_y + i * m_charHeight;
					fillRect.w = currentRowLength * m_charWidth;
					fillRect.h = m_charHeight;

					if (i == upperRow)
					{
						int offset = upperColumn * m_charWidth;

						fillRect.x += offset;
						fillRect.w -= offset;
					}

					if (i == lowerRow)
						fillRect.w -= (currentRowLength - lowerColumn) * m_charWidth;

					SDL_RenderFillRect(Game::get()->getRenderer(), &fillRect);
				}
			}
			else
			{
				SDL_Rect primaryCursorRect;
				primaryCursorRect.x = m_x + m_primaryColumn * m_charWidth;
				primaryCursorRect.y = m_y + m_primaryRow * m_charHeight;
				primaryCursorRect.w = m_cursorWidth;
				primaryCursorRect.h = m_charHeight;

				SDL_SetRenderDrawColor(Game::get()->getRenderer(), 0, 0, 0, 255);
				SDL_RenderFillRect(Game::get()->getRenderer(), &primaryCursorRect);
			}
		}

		FontManager::get()->draw(m_fontID, m_text, m_x, m_y, m_fr, m_fg, m_fb, m_fa, 0, 0, 0, 0);
	}

	void TextBox::onDownPressed()
	{
		if (m_primaryRow < getNumRows() - 1)
		{
			int rowLength = getRowLength(++m_primaryRow);

			if (m_primaryColumn > rowLength)
				m_primaryColumn = rowLength;
		}

		if (!InputHandler::get()->isKeyDown(SDL_SCANCODE_LSHIFT) && !InputHandler::get()->isKeyDown(SDL_SCANCODE_RSHIFT))
			resetSecondaryCursor();
	}

	void TextBox::onUpPressed()
	{
		if (m_primaryRow > 0)
		{
			int rowLength = getRowLength(--m_primaryRow);

			if (m_primaryColumn > rowLength)
				m_primaryColumn = rowLength;
		}

		if (!InputHandler::get()->isKeyDown(SDL_SCANCODE_LSHIFT) && !InputHandler::get()->isKeyDown(SDL_SCANCODE_RSHIFT))
			resetSecondaryCursor();
	}

	void TextBox::onRightPressed()
	{
		if (m_primaryColumn < getRowLength(m_primaryRow))
		{
			m_primaryColumn++;
		}
		else if (m_primaryRow < getNumRows() - 1)
		{
			m_primaryColumn = 0;
			m_primaryRow++;
		}

		if (!InputHandler::get()->isKeyDown(SDL_SCANCODE_LSHIFT) && !InputHandler::get()->isKeyDown(SDL_SCANCODE_RSHIFT))
			resetSecondaryCursor();
	}

	void TextBox::onLeftPressed()
	{
		if (m_primaryColumn > 0)
			m_primaryColumn--;
		else if (m_primaryRow > 0)
			m_primaryColumn = getRowLength(--m_primaryRow);

		if (!InputHandler::get()->isKeyDown(SDL_SCANCODE_LSHIFT) && !InputHandler::get()->isKeyDown(SDL_SCANCODE_RSHIFT))
			resetSecondaryCursor();
	}

	void TextBox::onDeletePressed()
	{
		if (isSelecting())
		{
			clearSelection();
		}
		else
		{
			int primaryCursorIndex = getCursorIndex(m_primaryRow, m_primaryColumn);

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
			if (m_primaryColumn > 0)
				m_primaryColumn--;
			else if (m_primaryRow > 0)
				m_primaryColumn = getRowLength(--m_primaryRow);
			else
				return;

			int primaryCursorIndex = getCursorIndex(m_primaryRow, m_primaryColumn);

			if (primaryCursorIndex < (int)m_text.size())
				m_text.erase(m_text.begin() + primaryCursorIndex);

			resetSecondaryCursor();
		}
	}

	void TextBox::onReturnPressed()
	{
		if (isSelecting())
			clearSelection();

		m_text.insert(getCursorIndex(m_primaryRow, m_primaryColumn), "\n");
		m_primaryColumn = 0;
		m_primaryRow++;

		resetSecondaryCursor();
	}

	void TextBox::onTabPressed()
	{
		if (isSelecting())
			clearSelection();

		m_text.insert(getCursorIndex(m_primaryRow, m_primaryColumn), "    ");
		m_primaryColumn += 4;

		resetSecondaryCursor();
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

	int TextBox::getCursorIndex(int row, int column)
	{
		std::vector<int> newlineIDs = getNewlineIndices();
		return newlineIDs[row] + column;
	}

	void TextBox::resetPrimaryCursor()
	{
		m_primaryRow = m_secondaryRow;
		m_primaryColumn = m_secondaryColumn;
	}

	void TextBox::resetSecondaryCursor()
	{
		m_secondaryRow = m_primaryRow;
		m_secondaryColumn = m_primaryColumn;
	}

	void TextBox::clearSelection()
	{
		int primaryCursorIndex = getCursorIndex(m_primaryRow, m_primaryColumn);
		int secondaryCursorIndex = getCursorIndex(m_secondaryRow, m_secondaryColumn);

		m_text.erase(m_text.begin() + min(primaryCursorIndex, secondaryCursorIndex), m_text.begin() + max(primaryCursorIndex, secondaryCursorIndex));

		if (secondaryCursorIndex < primaryCursorIndex)
			resetPrimaryCursor();
		else
			resetSecondaryCursor();
	}

	bool TextBox::isSelecting()
	{
		return m_secondaryRow != m_primaryRow || m_secondaryColumn != m_primaryColumn;
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
