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
		{
			Debugger::get()->log("Could not load font file \"" + file + "\"", Debugger::WARNING);
		}
		else
		{
			if (m_fonts.size() == 0)
				m_defaultFont = name;

			m_fonts[name] = TTF_OpenFont(file.c_str(), ptsize);
		}
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

	void FontManager::draw(std::string name, std::string text, int x, int y, int fgR, int fgG, int fgB, int fgA, int bgR, int bgG, int bgB, int bgA, int* width, int* height)
	{
		if (text.empty() || m_fonts.find(name) == m_fonts.end())
			return;

		SDL_Color fgColor;
		fgColor.r = fgR;
		fgColor.g = fgG;
		fgColor.b = fgB;

		SDL_Surface* pTextSurface = TTF_RenderText_Blended_Wrapped(m_fonts[name], text.c_str(), fgColor, Game::get()->getWidth());

		SDL_Texture* pTextTexture = SDL_CreateTextureFromSurface(Game::get()->getRenderer(), pTextSurface);

		SDL_QueryTexture(pTextTexture, NULL, NULL, width, height);

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

	void FontManager::draw(std::string text, int x, int y, int fgR, int fgG, int fgB, int fgA, int bgR, int bgG, int bgB, int bgA, int* width, int* height)
	{
		if (m_fonts.size() > 0)
			draw(m_defaultFont, text, x, y, fgR, fgG, fgB, fgA, bgR, bgG, bgB, bgA, width, height);
	}

	Vector2 FontManager::getCharacterSize(std::string name)
	{
		int width;
		int height;

		TTF_SizeText(m_fonts[name], "a", &width, &height);

		return Vector2((double)width, (double)height);
	}
}