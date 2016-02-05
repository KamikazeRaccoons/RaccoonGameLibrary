#include "Crate.h"

void Crate::onCreate()
{
	m_leafEmitter = rgl::Emitter(m_pLevel, "Leaf", 16, 16, 1.0f, 3.0f, 45.0f, 135.0f, 4.0f, 1, 3, 20, 40, rgl::Vector2(0.0f, 0.1f));
	m_poofEmitter = rgl::Emitter(m_pLevel, "Poof", 16, 16, 0.5, 2.0, 180.0f, 360.0f, 4.0f, 1, 2, 10, 20, rgl::Vector2());

	rgl::PhysicsObject::onCreate();

	b2PolygonShape shape;
	shape.SetAsBox(m_pLevel->toTileUnits(m_width) * 0.5f, m_pLevel->toTileUnits(m_height) * 0.5f);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.4f;
	fixtureDef.restitution = 0.2f;
	addFixture(&fixtureDef);
}

void Crate::update()
{
	rgl::PhysicsObject::update();

	int levelMousePosX = (int)m_pLevel->getLevelMousePosition().getX();
	int levelMousePosY = (int)m_pLevel->getLevelMousePosition().getY();

	if (rgl::InputHandler::get()->getMouseButtonState(rgl::InputHandler::RIGHT))
	{
		if (levelMousePosX < (m_x + m_width) &&
			levelMousePosX > m_x &&
			levelMousePosY < (m_y + m_height) &&
			levelMousePosY > m_y)
			m_pLevel->removeObject(shared_from_this());
	}
}

void Crate::draw()
{
	rgl::PhysicsObject::draw();
}

void Crate::onBeginContact(rgl::Vector2 contactPosition, rgl::PhysicsObject* pPhysicsObject)
{
	if (m_pBody->GetLinearVelocity().Length() < 4.0f)
		return;

	int realX = m_pLevel->toPixelUnits((float)contactPosition.getX());
	int realY = m_pLevel->toPixelUnits((float)contactPosition.getY());

	if (pPhysicsObject == 0)
		m_leafEmitter.spawn(contactPosition);
	else
		m_poofEmitter.spawn(contactPosition);
}