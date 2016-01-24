#pragma once

#include "stdafx.h"
#include "Layer.h"
#include "ObjectParams.h"

namespace rgl
{
	class GameObject;

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

		std::vector<Tileset> m_tilesets;
		std::vector<std::shared_ptr<Layer>> m_layers;
		std::vector<std::string> m_textureIDs;
		std::vector<std::function<void()>> m_callbacks;
		std::vector<std::tuple<OperationType, std::shared_ptr<GameObject>, int>> m_queuedOperations;

		Vector2 m_position;
		Vector2 m_velocity;

		b2Vec2 m_gravity;
		b2World m_world;

		RGL_API void pollOperations();

	public:

		Level() : m_position(0, 0), m_velocity(0, 0), m_gravity(0, -9.81f), m_world(m_gravity) { }
		~Level() { }

		RGL_API void update();
		RGL_API void render();
		RGL_API void clean();

		RGL_API void addCallback(std::function<void()> callback);
		RGL_API std::function<void()> getCallback(int callbackID);

		RGL_API void addLayer(std::shared_ptr<Layer> pLayer);

		RGL_API void addTexture(std::string file, std::string textureID);

		RGL_API void addObject(std::shared_ptr<GameObject> pObject, int objectLayer = -1);
		RGL_API void removeObject(std::shared_ptr<GameObject> pObject);

		RGL_API Tileset getTilesetByID(int tileID);

		RGL_API std::vector<Tileset>& getTilesets();

		RGL_API Vector2& getPosition();
		RGL_API Vector2& getVelocity();

		RGL_API b2Vec2& getGravity();

	};
}