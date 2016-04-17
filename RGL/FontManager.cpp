#include "FontManager.h"
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

	void FontManager::loadFont(std::string file, int ptsize, std::string name)
	{
		std::shared_ptr<FontData> pFontData = std::make_shared<FontData>(file, ptsize);

		if (pFontData->getFont() == 0)
			return;

		if (m_fontData.size() == 0)
			m_defaultFont = name;

		m_fontData[name] = pFontData;
	}

	void FontManager::unloadFont(std::string name)
	{
		m_fontData.erase(name);
	}

	void FontManager::clearFonts()
	{
		m_fontData.clear();
	}

	void FontManager::cacheText(std::string key, std::string font, std::string text, int r, int g, int b, int a)
	{
		if (m_fontData.find(font) == m_fontData.end() && key != m_TEMPKEY)
			return;

		SDL_Color textColor;
		textColor.r = r;
		textColor.g = g;
		textColor.b = b;

		SDL_Surface* pTextSurface = TTF_RenderText_Blended_Wrapped(m_fontData[font]->getFont(), text.c_str(), textColor, Game::get()->getWidth());
		SDL_Texture* pTextTexture = SDL_CreateTextureFromSurface(Game::get()->getRenderer(), pTextSurface);
		SDL_FreeSurface(pTextSurface);
		SDL_SetTextureAlphaMod(pTextTexture, a);

		m_textData[key] = std::make_shared<TextData>(pTextTexture);
	}

	void FontManager::cacheText(std::string key, std::string text, int r, int g, int b, int a)
	{
		if (m_fontData.size() > 0)
			cacheText(key, m_defaultFont, text, r, g, b, a);
	}

	void FontManager::drawText(std::string key, int x, int y)
	{
		if (m_textData.find(key) == m_textData.end())
			return;

		std::shared_ptr<TextData> textData = m_textData[key];

		SDL_Rect srcRect;
		SDL_Rect dstRect;

		srcRect.x = 0;
		srcRect.y = 0;
		srcRect.w = dstRect.w = textData->getTextWidth();
		srcRect.h = dstRect.h = textData->getTextHeight();
		dstRect.x = x;
		dstRect.y = y;

		SDL_RenderCopy(Game::get()->getRenderer(), textData->getTexture(), &srcRect, &dstRect);
	}

	void FontManager::drawText(std::string font, std::string text, int x, int y, int r, int g, int b, int a)
	{
		cacheText(m_TEMPKEY, font, text, r, g, b, a);
		drawText(m_TEMPKEY, x, y);
		freeText(m_TEMPKEY);
	}

	void FontManager::drawText(std::string text, int x, int y, int r, int g, int b, int a)
	{
		if (m_fontData.size() > 0)
			drawText(m_defaultFont, text, x, y, r, g, b, a);
	}

	void FontManager::freeText(std::string key)
	{
		m_textData.erase(key);
	}

	bool FontManager::isTextCached(std::string key)
	{
		return m_textData.find(key) != m_textData.end();
	}

	int FontManager::getTextWidth(std::string textKey)
	{
		return m_textData[textKey]->getTextWidth();
	}

	int FontManager::getTextHeight(std::string textKey)
	{
		return m_textData[textKey]->getTextHeight();
	}

	int FontManager::getCharWidth(std::string textKey)
	{
		return m_fontData[textKey]->getCharWidth();
	}

	int FontManager::getCharHeight(std::string textKey)
	{
		return m_fontData[textKey]->getCharHeight();
	}
}