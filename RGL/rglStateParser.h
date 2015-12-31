#pragma once

#include "stdafx.h"

#ifdef RGL_EXPORTS
#define RGL_STATEPARSER_API __declspec(dllexport)
#else
#define RGL_STATEPARSER_API __declspec(dllimport)
#endif

class rglGameObject;

class rglStateParser
{
private:

	RGL_STATEPARSER_API void parseObjects(XMLElement* pStateRoot, vector<shared_ptr<rglGameObject>>& objects);
	RGL_STATEPARSER_API void parseTextures(XMLElement* pTextureRoot, vector<string>& textureIDs);

public:

	RGL_STATEPARSER_API bool parseState(const char* file, string stateID, vector<shared_ptr<rglGameObject>>& objects,
		vector<string>& textureIDs);

};