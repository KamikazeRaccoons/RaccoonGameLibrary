#pragma once

#include "stdafx.h"
#include "Level.h"
#include "TileLayer.h"

#ifdef RGL_EXPORTS
#define RGL_LEVELPARSER_API __declspec(dllexport)
#else
#define RGL_LEVELPARSER_API __declspec(dllimport)
#endif

namespace rgl
{
	class LevelParser
	{
	private:

		int m_tileSize;
		int m_width;
		int m_height;

		RGL_LEVELPARSER_API void parseTilesets(tinyxml2::XMLElement* pTilesetRoot, std::shared_ptr<Level> pLevel, std::string path);
		RGL_LEVELPARSER_API void parseTileLayer(tinyxml2::XMLElement* pTileElement, std::shared_ptr<Level> pLevel);
		RGL_LEVELPARSER_API void parseTextures(tinyxml2::XMLElement* pPropertiesRoot, std::shared_ptr<Level> pLevel, std::string path);
		RGL_LEVELPARSER_API void parseObjectLayers(tinyxml2::XMLElement* pObjectGroupRoot, std::shared_ptr<Level> pLevel);

	public:

		RGL_LEVELPARSER_API std::shared_ptr<Level> parseLevel(std::string path, std::string file);

	};
}