#pragma once

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

		virtual void update();
		virtual void render();
		virtual void clean();

		std::vector<std::shared_ptr<GameObject>>& getGameObjects();

	};
}