#pragma once

#include "stdafx.h"
#include "Layer.h"
#include "GameObject.h"

namespace rgl
{
	class ObjectLayer : public Layer
	{
	private:

		std::vector<std::shared_ptr<GameObject>> m_gameObjects;

	public:

		ObjectLayer(std::shared_ptr<Level> pLevel) : Layer(pLevel) { }

		virtual RGL_API void update();
		virtual RGL_API void render();
		virtual RGL_API void clean();

		RGL_API std::vector<std::shared_ptr<GameObject>>& getGameObjects();

	};
}