#pragma once

#include "Level.h"
#include "TileLayer.h"
#include "tinyxml2.h"

namespace rgl
{
	class LevelParser
	{
	private:

		static void parseTilesets(tinyxml2::XMLElement* pTilesetRoot, std::shared_ptr<Level> pLevel, std::string path);
		static void parseTileLayer(tinyxml2::XMLElement* pTileElement, std::shared_ptr<Level> pLevel);
		static void parseProperties(tinyxml2::XMLElement* pPropertiesRoot, std::shared_ptr<Level> pLevel, std::string path);
		static void parseObjectLayers(tinyxml2::XMLElement* pObjectGroupRoot, std::shared_ptr<Level> pLevel);

	public:

		static std::shared_ptr<Level> parseLevel(std::string path, std::string file);

	};
}