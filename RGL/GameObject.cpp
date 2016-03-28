#include "GameObject.h"
#include "FontManager.h"
#include "Level.h"
#include "Game.h"

namespace rgl
{
	void GameObject::onCreate()
	{
		FontManager::get()->compileText(m_name, m_name, 0, 255, 0, 127);

		m_debugRect.w = FontManager::get()->getTextWidth(m_name);
		m_debugRect.h = FontManager::get()->getTextHeight(m_name);
	}

	void GameObject::onDestroy()
	{
		FontManager::get()->freeText(m_name);
	}

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
		m_debugRect.x = m_pLevel->toLevelPositionX(getX());
		m_debugRect.y = m_pLevel->toLevelPositionY(getY());
		
		SDL_SetRenderDrawColor(Game::get()->getRenderer(), 0, 0, 0, 127);
		SDL_RenderFillRect(Game::get()->getRenderer(), &m_debugRect);
		FontManager::get()->drawText(m_name, m_debugRect.x, m_debugRect.y);
	}
}