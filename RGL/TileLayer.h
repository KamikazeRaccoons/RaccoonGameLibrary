#pragma once

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
		std::vector<b2Body*> m_pBodies;

	public:

		TileLayer(std::shared_ptr<Level> pLevel);

		virtual void update() { }
		virtual void render();
		virtual void clean();

		void setTileIDs(const std::vector<std::vector<int>>& data);
		void setTileSize(int tileSize);
		void generateBodies();

	};
}