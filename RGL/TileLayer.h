#pragma once

#include "stdafx.h"
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

		RGL_API TileLayer(std::shared_ptr<Level> pLevel);

		virtual void update() { }
		virtual RGL_API void render();
		virtual RGL_API void clean();

		RGL_API void setTileIDs(const std::vector<std::vector<int>>& data);
		RGL_API void setTileSize(int tileSize);
		RGL_API void generateBodies();

	};
}