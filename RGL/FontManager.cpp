#include "FontManager.h"
#include "Debugger.h"
#include "Game.h"

namespace rgl
{
	FontManager* FontManager::m_pInstance = 0;

	FontManager* FontManager::get()
	{
		if (m_pInstance == 0)
			m_pInstance = new FontManager();

		return m_pInstance;
	}

	void FontManager::init()
	{
		if (!TTF_WasInit())
			TTF_Init();
	}

	void FontManager::load(std::string file, int ptsize, std::string name)
	{
		TTF_Font* pFont = TTF_OpenFont(file.c_str(), ptsize);

		if (pFont == 0)
			Debugger::get()->log("Could not load font file \"" + file + "\"", Debugger::WARNING);
		else
			m_fonts[name] = TTF_OpenFont(file.c_str(), ptsize);
	}

	void FontManager::unload(std::string name)
	{
		TTF_CloseFont(m_fonts[name]);
	}

	void FontManager::clear()
	{
		for (auto font : m_fonts)
			TTF_CloseFont(font.second);

		m_fonts.clear();
	}

	void FontManager::draw(std::string name, std::string text, int x, int y, int fgR, int fgG, int fgB, int fgA, int bgR, int bgG, int bgB, int bgA)
	{
		SDL_Color fgColor;
		fgColor.r = fgR;
		fgColor.g = fgG;
		fgColor.b = fgB;

		SDL_Surface* pTextSurface = TTF_RenderText_Solid(m_fonts[name], text.c_str(), fgColor);
		SDL_Texture* pTextTexture = SDL_CreateTextureFromSurface(Game::get()->getRenderer(), pTextSurface);

		SDL_SetTextureAlphaMod(pTextTexture, fgA);

		SDL_Rect srcRect;
		SDL_Rect dstRect;

		srcRect.x = 0;
		srcRect.y = 0;
		srcRect.w = dstRect.w = pTextSurface->w;
		srcRect.h = dstRect.h = pTextSurface->h;
		dstRect.x = x;
		dstRect.y = y;

		if (!(bgR == 0 && bgG == 0 && bgB == 0 && bgA == 0))
		{
			SDL_SetRenderDrawColor(Game::get()->getRenderer(), bgR, bgG, bgB, bgA);
			SDL_RenderFillRect(Game::get()->getRenderer(), &dstRect);
		}

		SDL_RenderCopy(Game::get()->getRenderer(), pTextTexture, &srcRect, &dstRect);

		SDL_FreeSurface(pTextSurface);
		SDL_DestroyTexture(pTextTexture);
	}

	void FontManager::draw(std::string text, int x, int y, int fgR, int fgG, int fgB, int fgA, int bgR, int bgG, int bgB, int bgA)
	{
		draw(m_fonts.begin()->first, text, x, y, fgR, fgG, fgB, fgA, bgR, bgG, bgB, bgA);
	}
}