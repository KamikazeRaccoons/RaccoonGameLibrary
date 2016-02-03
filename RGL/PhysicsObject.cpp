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
		bodyDef.position.Set(m_pLevel->toTileUnits(m_x + m_width / 2), m_pLevel->toTileUnits(m_y + m_height / 2));
		m_pBody = m_pLevel->getWorld()->CreateBody(&bodyDef);
		m_pBody->SetUserData(this);
	}

	void PhysicsObject::onDestroy()
	{
		for (auto fixture : m_fixtures)
			m_pBody->DestroyFixture(fixture);

		m_pLevel->getWorld()->DestroyBody(m_pBody);
	}

	void PhysicsObject::update()
	{
		m_x = m_pLevel->toPixelUnits(m_pBody->GetPosition().x) - m_width / 2;
		m_y = m_pLevel->toPixelUnits(m_pBody->GetPosition().y) - m_height / 2;
	}

	void PhysicsObject::draw()
	{
		TextureManager::get()->draw(m_textureID, m_pLevel->toLevelPositionX(m_x), m_pLevel->toLevelPositionY(m_y), m_width, m_height,
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

	void PhysicsObject::addFixture(b2FixtureDef* pFixtureDef)
	{
		m_fixtures.push_back(m_pBody->CreateFixture(pFixtureDef));
	}
}