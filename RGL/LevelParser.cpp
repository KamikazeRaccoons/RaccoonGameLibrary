#include "LevelParser.h"
#include "Debugger.h"
#include "ObjectLayer.h"
#include "ObjectFactory.h"
#include "base64.h"

#include <zlib.h>

namespace rgl
{
	void LevelParser::parseTilesets(tinyxml2::XMLElement* pTilesetRoot, std::shared_ptr<Level> pLevel, std::string path)
	{
		pLevel->addTexture(path + pTilesetRoot->FirstChildElement()->Attribute("source"), pTilesetRoot->Attribute("name"));

		Tileset tileset;
		tileset.width = pTilesetRoot->FirstChildElement()->IntAttribute("width");
		tileset.height = pTilesetRoot->FirstChildElement()->IntAttribute("height");
		tileset.firstGridID = pTilesetRoot->IntAttribute("firstgid");
		tileset.tileWidth = pTilesetRoot->IntAttribute("tilewidth");
		tileset.tileHeight = pTilesetRoot->IntAttribute("tileheight");
		tileset.spacing = pTilesetRoot->IntAttribute("spacing");
		tileset.margin = pTilesetRoot->IntAttribute("margin");
		tileset.name = pTilesetRoot->Attribute("name");
		tileset.numColumns = tileset.width / (tileset.tileWidth + tileset.spacing);

		pLevel->getTilesets().push_back(tileset);
	}

	void LevelParser::parseTileLayer(tinyxml2::XMLElement* pTileElement, std::shared_ptr<Level> pLevel)
	{
		std::shared_ptr<TileLayer> pTileLayer = std::make_shared<TileLayer>(pLevel);

		std::vector<std::vector<int>> data;

		bool isInteractive = false;

		std::string decodedIDs;
		tinyxml2::XMLElement* pDataElement = 0;

		for (tinyxml2::XMLElement* e = pTileElement->FirstChildElement(); e != 0; e = e->NextSiblingElement())
		{
			if (e->Value() == std::string("properties"))
			{
				for (tinyxml2::XMLElement* p = e->FirstChildElement(); p != 0; p = p->NextSiblingElement())
				{
					if (p->Name() == std::string("property"))
					{
						std::string nameAttribute = p->Attribute("name");

						if (nameAttribute == std::string("interactive"))
							isInteractive = p->BoolAttribute("value");
					}
				}
			}
			else if (e->Value() == std::string("data"))
			{
				pDataElement = e;
			}
		}

		if (!pDataElement)
		{
			Debugger::get()->log("Could not find \"data\" element.", Debugger::WARNING);
			return;
		}

		std::string t = pDataElement->FirstChild()->ToText()->Value();
		t.erase(remove_if(t.begin(), t.end(), isspace), t.end());
		decodedIDs = base64_decode(t);

		uLongf numGids = pLevel->getWidth() * pLevel->getHeight() * sizeof(int);
		std::vector<unsigned int> gids(numGids);
		uncompress((Bytef*)&gids[0], &numGids, (const Bytef*)decodedIDs.c_str(), decodedIDs.size());

		std::vector<int> layerRow(pLevel->getWidth());

		for (int i = 0; i < pLevel->getHeight(); i++)
			data.push_back(layerRow);

		for (int rows = 0; rows < pLevel->getHeight(); rows++)
			for (int columns = 0; columns < pLevel->getWidth(); columns++)
				data[rows][columns] = gids[rows * pLevel->getWidth() + columns];

		pTileLayer->setTileIDs(data);

		if (isInteractive)
			pTileLayer->generateBodies();

		pLevel->addLayer(pTileLayer);
	}

	void LevelParser::parseProperties(tinyxml2::XMLElement* pPropertiesRoot, std::shared_ptr<Level> pLevel, std::string path)
	{
		for (tinyxml2::XMLElement* e = pPropertiesRoot->FirstChildElement(); e != 0; e = e->NextSiblingElement())
		{
			if (e->Value() == std::string("property"))
			{
				std::string nameAttribute = e->Attribute("name");

				if (nameAttribute == "gravity")
					pLevel->getWorld()->SetGravity(b2Vec2(0.0f, e->FloatAttribute("value")));
				else
					pLevel->addTexture(path + e->Attribute("value"), nameAttribute);
			}
		}
	}

	void LevelParser::parseObjectLayers(tinyxml2::XMLElement* pObjectGroupRoot, std::shared_ptr<Level> pLevel)
	{
		pLevel->addLayer(std::make_shared<ObjectLayer>(pLevel));

		for (tinyxml2::XMLElement* e = pObjectGroupRoot->FirstChildElement(); e != 0; e = e->NextSiblingElement())
		{
			if (e->Value() == std::string("object"))
			{
				const char* nameAttribute = e->Attribute("name");
				const char* typeAttribute = e->Attribute("type");

				if (typeAttribute == 0)
				{
					Debugger::get()->log("Undefined type for object \"" + (nameAttribute == 0 ? "(unnamed GameObject)" : std::string(nameAttribute)) + "\".", Debugger::WARNING);
					continue;
				}

				std::shared_ptr<ObjectParams> pParams = std::make_shared<ObjectParams>();

				for (const tinyxml2::XMLAttribute* a = e->FirstAttribute(); a != 0; a = a->Next())
					pParams->setParam(a->Name(), a->Value());

				for (tinyxml2::XMLElement* properties = e->FirstChildElement(); properties != 0; properties = properties->NextSiblingElement())
				{
					if (properties->Value() == std::string("properties"))
					{
						for (tinyxml2::XMLElement* property = properties->FirstChildElement(); property != 0; property = property->NextSiblingElement())
						{
							if (property->Value() == std::string("property"))
								pParams->setParam(property->Attribute("name"), property->Attribute("value"));
						}
					}
				}

				if (nameAttribute == 0)
					pLevel->addObject(ObjectFactory::get()->create(typeAttribute, pParams));
				else
					pLevel->addObject(ObjectFactory::get()->create(typeAttribute, pParams, nameAttribute));
			}
		}
	}

	std::shared_ptr<Level> LevelParser::parseLevel(std::string path, std::string file)
	{
		tinyxml2::XMLDocument xmlDoc;
		xmlDoc.LoadFile((path + file).c_str());

		tinyxml2::XMLElement* pRoot = xmlDoc.RootElement();

		std::shared_ptr<Level> pLevel = std::make_shared<Level>(pRoot->IntAttribute("tilewidth"), pRoot->IntAttribute("width"), pRoot->IntAttribute("height"));

		for (tinyxml2::XMLElement* e = pRoot->FirstChildElement(); e != 0; e = e->NextSiblingElement())
		{
			if (e->Value() == std::string("properties"))
				parseProperties(e, pLevel, path);
			else if (e->Value() == std::string("tileset"))
				parseTilesets(e, pLevel, path);
			else if (e->Value() == std::string("layer"))
				parseTileLayer(e, pLevel);
			else if (e->Value() == std::string("objectgroup"))
				parseObjectLayers(e, pLevel);
		}

		return pLevel;
	}
}