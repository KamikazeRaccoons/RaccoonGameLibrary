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

		Vector2 m_position;
		Vector2 m_velocity;

		const std::vector<Tileset>& m_tilesets;

		std::vector<std::vector<int>> m_tileIDs;

	public:

		RGL_API TileLayer(int tileSize, const std::vector<Tileset>& tilesets);

		virtual RGL_API void update();
		virtual RGL_API void render();
		virtual void clean() { }

		RGL_API void setTileIDs(const std::vector<std::vector<int>>& data);
		RGL_API void setTileSize(int tileSize);

		RGL_API Tileset getTilesetByID(int tileID);

	};
}