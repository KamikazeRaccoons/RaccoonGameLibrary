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

		RGL_LEVELPARSER_API void parseTilesets(tinyxml2::XMLElement* pTilesetRoot, std::vector<Tileset>& tilesets, std::vector<std::string>& textureIDs, std::string path);
		RGL_LEVELPARSER_API void parseTileLayer(tinyxml2::XMLElement* pTileElement, std::vector<std::shared_ptr<Layer>>& layers,
			const std::vector<Tileset>& tilesets);
		RGL_LEVELPARSER_API void parseTextures(tinyxml2::XMLElement* pPropertiesRoot, std::vector<std::string>& textureIDs, std::string path);
		RGL_LEVELPARSER_API void parseObjectLayers(tinyxml2::XMLElement* pObjectGroupRoot, std::vector<std::shared_ptr<Layer>>& layers);

	public:

		RGL_LEVELPARSER_API std::shared_ptr<Level> parseLevel(std::string path, std::string file);

	};
}