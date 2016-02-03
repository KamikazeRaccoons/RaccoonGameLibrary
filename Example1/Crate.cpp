#include "Crate.h"

void Crate::onCreate()
{
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

	std::shared_ptr<rgl::Vector2> pMousePos = rgl::InputHandler::get()->getMousePosition();

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

	if (m_colliding)
	{
		SDL_SetRenderDrawColor(rgl::Game::get()->getRenderer(), 255, 0, 0, 255);
		SDL_Rect collisionRect;
		collisionRect.x = m_pLevel->toLevelPositionX(m_pLevel->toPixelUnits((float)m_contactPosition.getX())) - 4;
		collisionRect.y = m_pLevel->toLevelPositionY(m_pLevel->toPixelUnits((float)m_contactPosition.getY())) - 4;
		collisionRect.w = 8;
		collisionRect.h = 8;
		SDL_RenderFillRect(rgl::Game::get()->getRenderer(), &collisionRect);
	}
}

void Crate::onBeginContact(rgl::Vector2 contactPosition, rgl::PhysicsObject* pPhysicsObject)
{
	m_colliding = true;
	m_contactPosition = contactPosition;
}

void Crate::onEndContact(rgl::Vector2 contactPosition, rgl::PhysicsObject* pPhysicsObject)
{
	m_colliding = false;
}