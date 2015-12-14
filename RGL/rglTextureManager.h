#pragma once

#include "stdafx.h"

#ifdef RGL_EXPORTS
#define RGL_TEXTUREMANAGER_API __declspec(dllexport)
#else
#define RGL_TEXTUREMANAGER_API __declspec(dllimport)
#endif

class rglTextureManager
{
private:

	static rglTextureManager* m_pInstance;

	rglTextureManager() { }
	~rglTextureManager() { }

	std::map<std::string, SDL_Texture*> m_textures;

public:

	static RGL_TEXTUREMANAGER_API rglTextureManager* getInstance();

	RGL_TEXTUREMANAGER_API bool load(std::string fileName, std::string id);
	RGL_TEXTUREMANAGER_API void draw(std::string id, int x, int y, int width, int height, SDL_RendererFlip renderFlip = SDL_FLIP_NONE);
	RGL_TEXTUREMANAGER_API void drawFrame(std::string id, int x, int y, int width, int height, int currentRow, int currentFrame,
		SDL_RendererFlip renderFlip = SDL_FLIP_NONE);

};