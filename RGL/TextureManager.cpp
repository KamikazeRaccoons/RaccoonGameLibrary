#include "TextureManager.h"
#include "Game.h"

#include <SDL_image.h>

namespace rgl
{
	TextureManager* TextureManager::m_pInstance = 0;

	TextureManager* TextureManager::get()
	{
		if (m_pInstance == 0)
			m_pInstance = new TextureManager();

		return m_pInstance;
	}

	bool TextureManager::load(std::string fileName, std::string id)
	{
		SDL_Surface* pTempSurface = IMG_Load(fileName.c_str());

		if (pTempSurface == 0)
			return false;

		SDL_Texture* pTexture = SDL_CreateTextureFromSurface(Game::get()->getRenderer(), pTempSurface);

		SDL_FreeSurface(pTempSurface);

		if (pTexture != 0)
		{
			m_textures[id] = pTexture;
			return true;
		}

		return false;
	}

	void TextureManager::unload(std::string id)
	{
		SDL_DestroyTexture(m_textures[id]);
		m_textures.erase(id);
	}

	void TextureManager::draw(std::string id, int x, int y, int width, int height, int alpha, const double angle, const SDL_Point* center, SDL_RendererFlip renderFlip)
	{
		drawFrame(id, x, y, width, height, 0, 0, alpha, angle, center, renderFlip);
	}

	void TextureManager::drawFrame(std::string id, int x, int y, int width, int height, int currentRow, int currentFrame, int alpha,
		const double angle, const SDL_Point* center, SDL_RendererFlip renderFlip)
	{
		SDL_Rect srcRect;
		SDL_Rect destRect;

		srcRect.x = width * currentFrame;
		srcRect.y = height * currentRow;
		srcRect.w = destRect.w = width;
		srcRect.h = destRect.h = height;
		destRect.x = x;
		destRect.y = y;

		SDL_SetTextureAlphaMod(m_textures[id], alpha);
		SDL_RenderCopyEx(Game::get()->getRenderer(), m_textures[id], &srcRect, &destRect, angle, center, renderFlip);
	}

	void TextureManager::drawTile(std::string id, int margin, int spacing, int x, int y, int width, int height,
		int currentRow, int currentFrame)
	{
		SDL_Rect srcRect;
		SDL_Rect destRect;
		srcRect.x = margin + (spacing + width) * currentFrame;
		srcRect.y = margin + (spacing + height) * currentRow;
		srcRect.w = destRect.w = width;
		srcRect.h = destRect.h = height;
		destRect.x = x;
		destRect.y = y;

		SDL_RenderCopyEx(Game::get()->getRenderer(), m_textures[id], &srcRect, &destRect, 0, 0, SDL_FLIP_NONE);
	}
}