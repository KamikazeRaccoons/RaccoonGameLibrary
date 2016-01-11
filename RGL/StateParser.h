#pragma once

#include "stdafx.h"

#ifdef RGL_EXPORTS
#define RGL_STATEPARSER_API __declspec(dllexport)
#else
#define RGL_STATEPARSER_API __declspec(dllimport)
#endif

namespace rgl
{
	class GameObject;

	class StateParser
	{
	private:

		RGL_STATEPARSER_API void parseObjects(tinyxml2::XMLElement* pStateRoot, std::vector<std::shared_ptr<GameObject>>& objects);
		RGL_STATEPARSER_API void parseTextures(tinyxml2::XMLElement* pTextureRoot, std::vector<std::string>& textureIDs);

	public:

		RGL_STATEPARSER_API bool parseState(const char* file, std::string stateID, std::vector<std::shared_ptr<GameObject>>& objects,
			std::vector<std::string>& textureIDs);

	};
}