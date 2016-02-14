#include "GameObject.h"
#include "FontManager.h"
#include "Level.h"

namespace rgl
{
	void GameObject::setParentLevel(std::shared_ptr<Level> pParentLevel)
	{
		if (!m_pLevel)
			m_pLevel = pParentLevel;
	}

	void GameObject::setName(std::string name)
	{
		m_name = name;
	}

	std::string GameObject::getName()
	{
		return m_name;
	}

	int GameObject::getDiameter()
	{
		return (int)std::sqrt(getWidth() * getWidth() + getHeight() * getHeight());
	}

	void GameObject::debugDrawName()
	{
		FontManager::get()->draw(m_name, m_pLevel->toLevelPositionX(getX()), m_pLevel->toLevelPositionY(getY()),
			0, 255, 0, 127, 0, 0, 0, 127);
	}
}