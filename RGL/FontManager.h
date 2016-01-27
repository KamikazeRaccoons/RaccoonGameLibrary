#pragma once

#include "stdafx.h"

namespace rgl
{
	class FontManager
	{
	private:

		static FontManager* m_pInstance;

		std::map<std::string, TTF_Font*> m_fonts;

	public:

		static RGL_API FontManager* get();

		RGL_API void init();
		RGL_API void load(std::string file, int ptsize, std::string name);
		RGL_API void unload(std::string name);
		RGL_API void clear();
		RGL_API void draw(std::string name, std::string text, int x, int y, int r, int g, int b, int a);
		RGL_API void draw(std::string text, int x, int y, int r, int g, int b, int a);

	};
}