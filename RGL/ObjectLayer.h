#pragma once

#include "stdafx.h"
#include "Layer.h"
#include "GameObject.h"

#ifdef RGL_EXPORTS
#define RGL_OBJECTLAYER_API __declspec(dllexport)
#else
#define RGL_OBJECTLAYER_API __declspec(dllimport)
#endif

namespace rgl
{
	class ObjectLayer : public Layer
	{
	private:

		std::vector<std::shared_ptr<GameObject>> m_gameObjects;

	public:

		virtual RGL_OBJECTLAYER_API void update();
		virtual RGL_OBJECTLAYER_API void render();

		RGL_OBJECTLAYER_API std::vector<std::shared_ptr<GameObject>>& getGameObjects();

	};
}