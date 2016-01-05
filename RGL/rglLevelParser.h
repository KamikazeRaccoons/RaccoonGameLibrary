#pragma once

#include "stdafx.h"
#include "rglLevel.h"
#include "rglTileLayer.h"

#ifdef RGL_EXPORTS
#define RGL_LEVELPARSER_API __declspec(dllexport)
#else
#define RGL_LEVELPARSER_API __declspec(dllimport)
#endif

class rglLevelParser
{
private:

	int m_tileSize;
	int m_width;
	int m_height;

	RGL_LEVELPARSER_API void parseTilesets(XMLElement* pTilesetRoot, vector<rglTileset>& tilesets, string texturePath);
	RGL_LEVELPARSER_API void parseTileLayer(XMLElement* pTileElement, vector<shared_ptr<rglLayer>>& layers,
		const vector<rglTileset>& tilesets);

public:

	RGL_LEVELPARSER_API shared_ptr<rglLevel> parseLevel(const char* file, string texturePath);

};