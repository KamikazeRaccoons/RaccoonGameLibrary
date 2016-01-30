#pragma once

#include <vector>
#include <memory>
#include <tuple>
#include <functional>

#include "Layer.h"
#include "ObjectParams.h"
#include "DebugDraw.h"
#include "Vector2.h"

namespace rgl
{
	class GameObject;
	class Layer;

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
	
	class Level : public std::enable_shared_from_this<Level>
	{
	private:

		enum OperationType
		{
			ADD = 0,
			REMOVE = 1
		};

		const int m_tileSize;
		const int m_width;
		const int m_height;

		std::vector<Tileset> m_tilesets;
		std::vector<std::shared_ptr<Layer>> m_layers;
		std::vector<std::string> m_textureIDs;
		std::vector<std::function<void()>> m_callbacks;
		std::vector<std::tuple<OperationType, std::shared_ptr<GameObject>, int>> m_queuedOperations;

		Vector2 m_position;
		Vector2 m_velocity;

		b2World* m_pWorld;

		DebugDraw m_debugDraw;

		void pollOperations();

	public:

		Level(int tileSize, int width, int height);
		~Level() { }

		void update();
		void render();
		void clean();

		int getTileSize() const;
		int getWidth() const;
		int getHeight() const;

		void addCallback(std::function<void()> callback);
		std::function<void()> getCallback(int callbackID);

		void addLayer(std::shared_ptr<Layer> pLayer);

		void addTexture(std::string file, std::string textureID);

		void addObject(std::shared_ptr<GameObject> pObject, int objectLayer = -1);
		void removeObject(std::shared_ptr<GameObject> pObject);

		Tileset getTilesetByID(int tileID);

		std::vector<Tileset>& getTilesets();

		Vector2& getPosition();
		Vector2& getVelocity();

		b2World* getWorld();

	};
}