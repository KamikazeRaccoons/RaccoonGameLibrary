#pragma once

#include "GameObject.h"

#include <vector>

namespace rgl
{
	class TextBox : public GameObject
	{
	protected:

		bool m_enabled;

		int m_x;
		int m_y;

		int m_width;
		int m_height;

		int m_fr;
		int m_fg;
		int m_fb;
		int m_fa;

		int m_br;
		int m_bg;
		int m_bb;
		int m_ba;

		std::string m_fontID;
		std::string m_text;

		int m_cursorWidth;

		int m_currentRow;
		int m_currentColumn;

		int m_charWidth;
		int m_charHeight;

		std::vector<int> getNewlineIndices();

		int getNumRows();
		int getRowLength(int row);
		int getCursorIndex();

	public:

		TextBox(bool enabled, int x, int y, int width, int height, int cursorWidth,
			int fr, int fg, int fb, int fa,
			int br, int bg, int bb, int ba,
			std::string fontID, std::string name = "(unnamed TextBox)");

		virtual void onCreate();
		virtual void onDestroy();

		virtual void update();
		virtual void draw();

		virtual int getX() { return m_x; };
		virtual int getY() { return m_y; };

		virtual int getWidth() { return m_width; };
		virtual int getHeight() { return m_height; };

		void setText(std::string text);
		std::string getText();

	};
}
