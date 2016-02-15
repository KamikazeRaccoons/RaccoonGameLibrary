#pragma once

#include <map>
#include <string>

#include <SDL_ttf.h>

#include "Vector2.h"

namespace rgl
{
	class FontManager
	{
	private:

		static FontManager* m_pInstance;

		std::map<std::string, TTF_Font*> m_fonts;

		std::string m_defaultFont;

	public:

		static FontManager* get();

		void init();
		void load(std::string file, int ptsize, std::string name);
		void unload(std::string name);
		void clear();
		void draw(std::string name, std::string text, int x, int y, int fgR, int fgG, int fgB, int fgA, int bgR = 0, int bgG = 0, int bgB = 0, int bgA = 0,
			int* width = 0, int* height = 0);
		void draw(std::string text, int x, int y, int fgR, int fgG, int fgB, int fgA, int bgR = 0, int bgG = 0, int bgB = 0, int bgA = 0,
			int* width = 0, int* height = 0);

		Vector2 getCharacterSize(std::string name);
	};
}