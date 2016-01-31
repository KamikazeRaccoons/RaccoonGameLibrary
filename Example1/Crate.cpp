#include "Crate.h"

void Crate::onCreate()
{
	rgl::PhysicsObject::onCreate();

	b2PolygonShape shape;
	shape.SetAsBox((float)m_width / (float)m_pLevel->getTileSize() * 0.5f, (float)m_height / (float)m_pLevel->getTileSize() * 0.5f);

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

	std::shared_ptr<rgl::Vector2> pMousePos = rgl::InputHandler::get()->getMousePosition();

	int levelMousePosX = (int)(pMousePos->getX() + m_pLevel->getPosition().getX());
	int levelMousePosY = (int)(pMousePos->getY() + m_pLevel->getPosition().getY());

	if (rgl::InputHandler::get()->getMouseButtonState(rgl::InputHandler::RIGHT))
	{
		if (levelMousePosX < (m_x + m_width) &&
			levelMousePosX > m_x &&
			levelMousePosY < (m_y + m_height) &&
			levelMousePosY > m_y)
			m_pLevel->removeObject(shared_from_this());
	}
}