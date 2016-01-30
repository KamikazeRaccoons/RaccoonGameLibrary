#include "ObjectLayer.h"
#include "Game.h"

namespace rgl
{
	std::vector<std::shared_ptr<GameObject>>& ObjectLayer::getGameObjects()
	{
		return m_gameObjects;
	}

	void ObjectLayer::update()
	{
		for (auto object : m_gameObjects)
			object->update();
	}

	void ObjectLayer::render()
	{
		for (auto object : m_gameObjects)
		{
			int radius = object->getDiameter();

			if (object->getX() + radius > m_pLevel->getPosition().getX() &&
				object->getX() - radius < m_pLevel->getPosition().getX() + Game::get()->getWidth() &&
				object->getY() + radius > m_pLevel->getPosition().getY() &&
				object->getY() - radius < m_pLevel->getPosition().getY() + Game::get()->getHeight())
			{
				object->draw();

				if (Game::get()->isDebugEnabled())
					object->debugDrawName();
			}
		}
	}

	void ObjectLayer::clean()
	{
		for (auto object : m_gameObjects)
			object->onDestroy();

		m_gameObjects.clear();
	}
}