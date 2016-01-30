#pragma once

#include <map>
#include <string>

#include <SDL.h>

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

		static TextureManager* get();

		bool load(std::string fileName, std::string id);
		void unload(std::string id);

		void draw(std::string id, int x, int y, int width, int height, const double angle = 0.0,
			const SDL_Point* center = 0, SDL_RendererFlip renderFlip = SDL_FLIP_NONE);
		void drawFrame(std::string id, int x, int y, int width, int height, int currentRow, int currentFrame,
			const double angle = 0.0, const SDL_Point* center = 0, SDL_RendererFlip renderFlip = SDL_FLIP_NONE);
		void drawTile(std::string id, int margin, int spacing, int x, int y, int width, int height,
			int currentRow, int currentFrame);

	};
}