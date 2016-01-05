#pragma once

#include "stdafx.h"
#include "rglLayer.h"
#include "rglLevel.h"
#include "rglVector2.h"

#ifdef RGL_EXPORTS
#define RGL_TILELAYER_API __declspec(dllexport)
#else
#define RGL_TILELAYER_API __declspec(dllimport)
#endif

class rglTileLayer : public rglLayer
{
private:

	int m_numColumns;
	int m_numRows;
	int m_tileSize;

	rglVector2 m_position;
	rglVector2 m_velocity;

	const vector<rglTileset>& m_tilesets;

	vector<vector<int>> m_tileIDs;

public:

	RGL_TILELAYER_API rglTileLayer(int tileSize, const vector<rglTileset>& tilesets);

	virtual RGL_TILELAYER_API void update();
	virtual RGL_TILELAYER_API void render();

	RGL_TILELAYER_API void setTileIDs(const vector<vector<int>>& data);
	RGL_TILELAYER_API void setTileSize(int tileSize);

	RGL_TILELAYER_API rglTileset getTilesetByID(int tileID);

};