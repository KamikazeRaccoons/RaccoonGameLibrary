#pragma once

#include "stdafx.h"
#include "Layer.h"
#include "Level.h"
#include "Vector2.h"

#ifdef RGL_EXPORTS
#define RGL_TILELAYER_API __declspec(dllexport)
#else
#define RGL_TILELAYER_API __declspec(dllimport)
#endif

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

		RGL_TILELAYER_API TileLayer(int tileSize, const std::vector<Tileset>& tilesets);

		virtual RGL_TILELAYER_API void update();
		virtual RGL_TILELAYER_API void render();
		virtual void clean() { }

		RGL_TILELAYER_API void setTileIDs(const std::vector<std::vector<int>>& data);
		RGL_TILELAYER_API void setTileSize(int tileSize);

		RGL_TILELAYER_API Tileset getTilesetByID(int tileID);

	};
}