#include "Emitter.h"
#include "Particle.h"
#include "MathHelper.h"

namespace rgl
{
	void Emitter::spawn(Vector2 position)
	{
		int numParticles = MathHelper::randInt(m_minParticleCount, m_maxParticleCount);

		for (int i = 0; i < numParticles; i++)
		{
			float angle = MathHelper::randFloat(m_minAngle, m_maxAngle);
			float radAngle = MathHelper::toRad(angle);
			float speed = MathHelper::randFloat(m_minSpeed, m_maxSpeed);

			m_pLevel->addObject(std::make_shared<Particle>(m_pLevel->toPixelUnits((float)position.getX()), m_pLevel->toPixelUnits((float)position.getY()),
				m_width, m_height, angle, Vector2(cosf(radAngle) * speed, -sinf(radAngle) * speed), m_acceleration,
				MathHelper::randFloat(-m_maxAngularVelocity, m_maxAngularVelocity), MathHelper::randInt(m_minLifetime, m_maxLifetime),
				m_textureID));
		}
	}
}