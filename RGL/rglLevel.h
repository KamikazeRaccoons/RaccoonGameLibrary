#pragma once

#include "stdafx.h"
#include "rglLayer.h"

#ifdef RGL_EXPORTS
#define RGL_LEVEL_API __declspec(dllexport)
#else
#define RGL_LEVEL_API __declspec(dllimport)
#endif

struct rglTileset
{
	int firstGridID;
	int tileWidth;
	int tileHeight;
	int spacing;
	int margin;
	int width;
	int height;
	int numColumns;
	string name;
};

class rglLevel
{
private:

	vector<rglTileset> m_tilesets;
	vector<shared_ptr<rglLayer>> m_layers;

public:

	rglLevel() { }
	~rglLevel() { }

	RGL_LEVEL_API void update();
	RGL_LEVEL_API void render();

	RGL_LEVEL_API vector<rglTileset>& getTilesets();
	RGL_LEVEL_API vector<shared_ptr<rglLayer>>& getLayers();

};