#pragma once

#include "stdafx.h"
#include "Level.h"
#include "TileLayer.h"

namespace rgl
{
	class LevelParser
	{
	private:

		static RGL_API void parseTilesets(tinyxml2::XMLElement* pTilesetRoot, std::shared_ptr<Level> pLevel, std::string path);
		static RGL_API void parseTileLayer(tinyxml2::XMLElement* pTileElement, std::shared_ptr<Level> pLevel);
		static RGL_API void parseProperties(tinyxml2::XMLElement* pPropertiesRoot, std::shared_ptr<Level> pLevel, std::string path);
		static RGL_API void parseObjectLayers(tinyxml2::XMLElement* pObjectGroupRoot, std::shared_ptr<Level> pLevel);

	public:

		static RGL_API std::shared_ptr<Level> parseLevel(std::string path, std::string file);

	};
}