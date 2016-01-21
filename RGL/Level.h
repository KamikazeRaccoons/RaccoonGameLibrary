#pragma once

#include "stdafx.h"
#include "Layer.h"
#include "ObjectParams.h"

#ifdef RGL_EXPORTS
#define RGL_LEVEL_API __declspec(dllexport)
#else
#define RGL_LEVEL_API __declspec(dllimport)
#endif

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
	
	class Level
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

		RGL_LEVEL_API void pollOperations();

	public:

		Level() { }
		~Level() { }

		RGL_LEVEL_API void update();
		RGL_LEVEL_API void render();
		RGL_LEVEL_API void clean();

		RGL_LEVEL_API void addCallback(std::function<void()> callback);
		RGL_LEVEL_API std::function<void()> getCallback(int callbackID);

		RGL_LEVEL_API void addLayer(std::shared_ptr<Layer> pLayer);

		RGL_LEVEL_API void addTexture(std::string file, std::string textureID);

		RGL_LEVEL_API void addObject(std::shared_ptr<GameObject> pObject, int objectLayer = -1);
		RGL_LEVEL_API void removeObject(std::shared_ptr<GameObject> pObject);

		RGL_LEVEL_API std::vector<Tileset>& getTilesets();

	};
}