#pragma once

#include <map>
#include <string>
#include <memory>

#include <SDL_ttf.h>

#include "Vector2.h"
#include "Debugger.h"

namespace rgl
{
	class FontManager
	{
	private:

		class FontData
		{
		private:

			TTF_Font* m_pFont;

			int m_charWidth;
			int m_charHeight;

		public:

			FontData(std::string file, int ptsize)
			{
				m_pFont = TTF_OpenFont(file.c_str(), ptsize);

				if (m_pFont == 0)
					Debugger::get()->log("Could not load font file \"" + file + "\"", Debugger::WARNING);
				else
					TTF_SizeText(m_pFont, " ", &m_charWidth, &m_charHeight);
			}

			~FontData()
			{
				TTF_CloseFont(m_pFont);
			}

			TTF_Font* getFont() { return m_pFont; }

			int getCharWidth() { return m_charWidth; }
			int getCharHeight() { return m_charHeight; }

		};

		class TextData
		{
		private:

			SDL_Texture* m_pTexture;

			int m_textWidth;
			int m_textHeight;

		public:

			TextData(SDL_Texture* pTexture)
			{
				m_pTexture = pTexture;
				SDL_QueryTexture(m_pTexture, NULL, NULL, &m_textWidth, &m_textHeight);
			}

			~TextData()
			{
				SDL_DestroyTexture(m_pTexture);
			}

			SDL_Texture* getTexture() { return m_pTexture; }

			int getTextWidth() { return m_textWidth; }
			int getTextHeight() { return m_textHeight; }
		};

		static FontManager* m_pInstance;

		const std::string m_TEMPKEY = "temp";

		std::map<std::string, std::shared_ptr<FontData>> m_fontData;
		std::map<std::string, std::shared_ptr<TextData>> m_textData;

		std::string m_defaultFont;

	public:

		static FontManager* get();

		void init();

		void loadFont(std::string file, int ptsize, std::string name);
		void unloadFont(std::string name);
		void clearFonts();

		void cacheText(std::string key, std::string font, std::string text, int r, int g, int b, int a);
		void cacheText(std::string key, std::string text, int r, int g, int b, int a);
		void drawText(std::string key, int x, int y);
		void drawText(std::string font, std::string text, int x, int y, int r, int g, int b, int a);
		void drawText(std::string text, int x, int y, int r, int g, int b, int a);
		void freeText(std::string key);

		bool isTextCached(std::string key);

		int getCharWidth(std::string textKey);
		int getCharHeight(std::string textKey);

		int getTextWidth(std::string textKey);
		int getTextHeight(std::string textKey);
	};
}