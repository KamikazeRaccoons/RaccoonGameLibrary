#include "Level.h"
#include "ObjectLayer.h"
#include "Game.h"
#include "TextureManager.h"
#include "Debugger.h"

namespace rgl
{
	Level::Level(int tileSize, int width, int height) : m_tileSize(tileSize), m_width(width), m_height(height),
		m_position(0, 0), m_velocity(0, 0), m_debugDraw(this)
	{
		m_pWorld = new b2World(b2Vec2(0.0f, 9.81f));
		m_debugDraw.AppendFlags(b2Draw::e_shapeBit);
		m_pWorld->SetDebugDraw(&m_debugDraw);
	}

	void Level::pollOperations()
	{
		for (auto operation : m_queuedOperations)
		{
			switch (std::get<0>(operation))
			{
			case ADD:
			{
				std::vector<std::shared_ptr<ObjectLayer>> objectLayers;

				for (auto layer : m_layers)
				{
					std::shared_ptr<ObjectLayer> pObjectLayer = std::dynamic_pointer_cast<ObjectLayer>(layer);

					if (pObjectLayer)
						objectLayers.push_back(pObjectLayer);
				}

				if (std::get<2>(operation) < (int)objectLayers.size())
				{
					if (std::get<2>(operation) < 0)
						objectLayers.back()->getGameObjects().push_back(std::get<1>(operation));
					else
						objectLayers[std::get<2>(operation)]->getGameObjects().push_back(std::get<1>(operation));

					std::get<1>(operation)->onCreate();
				}
			}
			break;
			case REMOVE:
				for (auto layer : m_layers)
				{
					std::shared_ptr<ObjectLayer> pObjectLayer = std::dynamic_pointer_cast<ObjectLayer>(layer);

					if (pObjectLayer)
					{
						std::vector<std::shared_ptr<GameObject>>& gameObjects = pObjectLayer->getGameObjects();

						if (std::find(gameObjects.begin(), gameObjects.end(), std::get<1>(operation)) != gameObjects.end())
						{
							std::get<1>(operation)->onDestroy();
							gameObjects.erase(std::remove(gameObjects.begin(), gameObjects.end(), std::get<1>(operation)), gameObjects.end());
						}
					}
				}
				break;
			}
		}

		m_queuedOperations.clear();
	}

	void Level::update()
	{
		m_position += m_velocity;

		pollOperations();

		m_pWorld->Step((float)Game::get()->getDeltaTime(), 8, 3);

		for (auto layer : m_layers)
			layer->update();
	}

	void Level::render()
	{
		for (auto layer : m_layers)
			layer->render();

		if (Game::get()->isDebugEnabled())
			m_pWorld->DrawDebugData();
	}

	void Level::clean()
	{
		for (auto layer : m_layers)
			layer->clean();

		m_layers.clear();

		for (auto textureID : m_textureIDs)
			TextureManager::get()->unload(textureID);

		m_textureIDs.clear();

		delete m_pWorld;
	}

	int Level::getTileSize() const
	{
		return m_tileSize;
	}

	int Level::getWidth() const
	{
		return m_width;
	}

	int Level::getHeight() const
	{
		return m_height;
	}

	void Level::addCallback(std::function<void()> callback)
	{
		m_callbacks.push_back(callback);
	}

	std::function<void()> Level::getCallback(int callbackID)
	{
		if (callbackID >= (int)m_callbacks.size())
			return std::function<void()>();

		return m_callbacks[callbackID];
	}

	void Level::addLayer(std::shared_ptr<Layer> pLayer)
	{
		m_layers.push_back(pLayer);
	}

	void Level::addTexture(std::string file, std::string textureID)
	{
		if (TextureManager::get()->load(file, textureID))
			m_textureIDs.push_back(textureID);
		else
			Debugger::get()->log("Could not load texture \"" + file + "\".", Debugger::WARNING);
	}

	void Level::addObject(std::shared_ptr<GameObject> pObject, int objectLayer)
	{
		pObject->setParentLevel(shared_from_this());
		m_queuedOperations.push_back(std::make_tuple(ADD, pObject, objectLayer));
	}

	void Level::removeObject(std::shared_ptr<GameObject> pObject)
	{
		m_queuedOperations.push_back(std::make_tuple(REMOVE, pObject, 0));
	}

	Tileset Level::getTilesetByID(int tileID)
	{
		for (unsigned int i = 0; i < m_tilesets.size(); i++)
		{
			if (i + 1 <= m_tilesets.size() - 1)
			{
				if (tileID >= m_tilesets[i].firstGridID && tileID < m_tilesets[i + 1].firstGridID)
					return m_tilesets[i];
			}
			else
			{
				return m_tilesets[i];
			}
		}

		Debugger::get()->log("Could not find tileset with ID " + std::to_string(tileID) + ". Returning empty tileset.");

		return Tileset();
	}

	std::vector<Tileset>& Level::getTilesets()
	{
		return m_tilesets;
	}

	Vector2& Level::getPosition()
	{
		return m_position;
	}

	Vector2& Level::getVelocity()
	{
		return m_velocity;
	}

	b2World* Level::getWorld()
	{
		return m_pWorld;
	}
}