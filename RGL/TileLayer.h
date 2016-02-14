#pragma once

#include <vector>

#include <Box2D\Box2D.h>

#include "Layer.h"
#include "Level.h"
#include "Vector2.h"

namespace rgl
{
	class TileLayer : public Layer
	{
	private:

		int m_numColumns;
		int m_numRows;
		int m_tileSize;

		std::vector<std::vector<int>> m_tileIDs;

		b2Body* m_pBody;
		std::vector<b2Fixture*> m_fixtures;

	public:

		TileLayer(std::shared_ptr<Level> pLevel);

		virtual void update() { }
		virtual void render();
		virtual void clean();

		void setTileIDs(const std::vector<std::vector<int>>& data);
		void setTileSize(int tileSize);
		void generateFixtures();

		bool isTileAt(int x, int y);

	};
}