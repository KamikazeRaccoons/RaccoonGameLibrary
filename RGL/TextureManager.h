#pragma once

#include "stdafx.h"

namespace rgl
{
	class TextureManager
	{
	private:

		static TextureManager* m_pInstance;

		TextureManager() { }
		~TextureManager() { }

		std::map<std::string, SDL_Texture*> m_textures;

	public:

		static RGL_API TextureManager* get();

		RGL_API bool load(std::string fileName, std::string id);
		RGL_API void unload(std::string id);

		RGL_API void draw(std::string id, int x, int y, int width, int height, SDL_RendererFlip renderFlip = SDL_FLIP_NONE);
		RGL_API void drawFrame(std::string id, int x, int y, int width, int height, int currentRow, int currentFrame,
			SDL_RendererFlip renderFlip = SDL_FLIP_NONE);
		RGL_API void drawTile(std::string id, int margin, int spacing, int x, int y, int width, int height,
			int currentRow, int currentFrame);

	};
}