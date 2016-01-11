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

	public:

		Level() { }
		~Level() { }

		RGL_LEVEL_API void update();
		RGL_LEVEL_API void render();

		RGL_LEVEL_API std::vector<Tileset>& getTilesets();
		RGL_LEVEL_API std::vector<std::shared_ptr<Layer>>& getLayers();

	};
}