#pragma once

#include "GameObject.h"

namespace rgl
{
	class TextBox : public GameObject
	{
	private:

		bool m_enabled;

		int m_x;
		int m_y;

		int m_width;
		int m_height;

		int m_r;
		int m_g;
		int m_b;
		int m_a;

		std::string m_fontID;
		std::string m_text;

		int m_cursorPosition;

	public:

		TextBox(bool enabled, int x, int y, int width, int height, int r, int g, int b, int a, std::string fontID, std::string name = "(unnamed TextBox)")
			: GameObject(name), m_enabled(enabled), m_x(x), m_y(y), m_width(width), m_height(height), m_r(r), m_g(g), m_b(b), m_a(a), m_fontID(fontID), m_cursorPosition(0) { }

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
