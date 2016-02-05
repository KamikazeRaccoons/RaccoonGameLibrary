#include "Particle.h"
#include "Level.h"
#include "TextureManager.h"

namespace rgl
{
	Particle::Particle(int x, int y, int width, int height, float angle,
		rgl::Vector2 velocity, rgl::Vector2 acceleration, float angularVelocity,
		int lifetime, std::string textureID, std::string name)
		: rgl::SimpleActor(x, y, width, height, angle, textureID, name), m_lifetime(lifetime), m_lifeLeft(lifetime)
	{
		m_velocity = velocity;
		m_acceleration = acceleration;
		m_angularVelocity = angularVelocity;
	}

	void Particle::update()
	{
		rgl::SimpleActor::update();

		if (--m_lifeLeft <= 0)
			m_pLevel->removeObject(shared_from_this());
	}

	void Particle::draw()
	{
		TextureManager::get()->draw(m_textureID, m_pLevel->toLevelPositionX((int)m_position.getX()), m_pLevel->toLevelPositionY((int)m_position.getY()),
			m_width, m_height, (int)((float)m_lifeLeft / (float)m_lifetime * 255.0f), m_angle);
	}
}
