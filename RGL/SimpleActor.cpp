#include "SimpleActor.h"
#include "TextureManager.h"
#include "Level.h"

namespace rgl
{
	void SimpleActor::update()
	{
		m_velocity += m_acceleration;
		m_position += m_velocity;
		m_angle += m_angularVelocity;
	}

	void SimpleActor::draw()
	{
		TextureManager::get()->draw(m_textureID, m_pLevel->toLevelPositionX((int)m_position.getX()), m_pLevel->toLevelPositionY((int)m_position.getY()),
			m_width, m_height, 255, m_angle);
	}
}