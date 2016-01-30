#include "PhysicsObject.h"
#include "Level.h"
#include "TextureManager.h"
#include "MathHelper.h"

namespace rgl
{
	void PhysicsObject::onCreate()
	{
		b2BodyDef bodyDef;
		bodyDef.type = m_bodyType;
		bodyDef.position.Set((float)(m_x + m_width / 2) / (float)m_pLevel->getTileSize(), (float)(m_y + m_height / 2) / (float)m_pLevel->getTileSize());
		m_pBody = m_pLevel->getWorld()->CreateBody(&bodyDef);
	}

	void PhysicsObject::onDestroy()
	{
		m_pLevel->getWorld()->DestroyBody(m_pBody);
	}

	void PhysicsObject::update()
	{
		m_x = (int)(m_pBody->GetPosition().x * m_pLevel->getTileSize()) - m_width / 2;
		m_y = (int)(m_pBody->GetPosition().y * m_pLevel->getTileSize()) - m_height / 2;
	}

	void PhysicsObject::draw()
	{
		TextureManager::get()->draw(m_textureID, m_x - (int)m_pLevel->getPosition().getX(), m_y - (int)m_pLevel->getPosition().getY(), m_width, m_height,
			MathHelper::toDeg(m_pBody->GetAngle()));
	}

	int PhysicsObject::getX()
	{
		return m_x;
	}

	int PhysicsObject::getY()
	{
		return m_y;
	}

	int PhysicsObject::getWidth()
	{
		return m_width;
	}

	int PhysicsObject::getHeight()
	{
		return m_height;
	}
}