#pragma once

#include "stdafx.h"
#include "Layer.h"

#ifdef RGL_EXPORTS
#define RGL_LEVEL_API __declspec(dllexport)
#else
#define RGL_LEVEL_API __declspec(dllimport)
#endif

namespace rgl
{
	struct Tileset
	{
		int firstGridID;
		int tileWidth;
		int tileHeight;
		int spacing;
		int margin;
		int width;
		int height;
		int numColumns;
		std::string name;
	};

	class Level
	{
	private:

		std::vector<Tileset> m_tilesets;
		std::vector<std::shared_ptr<Layer>> m_layers;
		std::vector<std::string> m_textureIDs;
		std::vector<std::function<void()>> m_callbacks;

	public:

		Level() { }
		~Level() { }

		RGL_LEVEL_API void update();
		RGL_LEVEL_API void render();
		RGL_LEVEL_API void clean();

		RGL_LEVEL_API void addCallback(std::function<void()> callback);
		RGL_LEVEL_API void assignCallbacks();

		RGL_LEVEL_API std::vector<Tileset>& getTilesets();
		RGL_LEVEL_API std::vector<std::shared_ptr<Layer>>& getLayers();
		RGL_LEVEL_API std::vector<std::string>& getTextureIDs();

	};
}