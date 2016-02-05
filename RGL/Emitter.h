#pragma once

#include <string>
#include <memory>

#include "Vector2.h"
#include "Level.h"

namespace rgl
{
	class Emitter
	{
	private:

		std::shared_ptr<Level> m_pLevel;

		std::string m_textureID;

		float m_minSpeed;
		float m_maxSpeed;
		float m_minAngle;
		float m_maxAngle;
		float m_maxAngularVelocity;

		int m_width;
		int m_height;
		int m_minParticleCount;
		int m_maxParticleCount;
		int m_minLifetime;
		int m_maxLifetime;

		Vector2 m_acceleration;

	public:

		Emitter()
			: m_pLevel(0), m_textureID(std::string()) { }

		Emitter(std::shared_ptr<Level> pLevel, std::string textureID, int width, int height, float minSpeed, float maxSpeed,
			float minAngle, float maxAngle, float maxAngularVelocity, int minParticleCount, int maxParticleCount,
			int minLifetime, int maxLifetime, Vector2 acceleration)
			: m_pLevel(pLevel), m_textureID(textureID), m_width(width), m_height(height), m_minSpeed(minSpeed), m_maxSpeed(maxSpeed),
			m_minAngle(minAngle), m_maxAngle(maxAngle), m_maxAngularVelocity(maxAngularVelocity),
			m_minParticleCount(minParticleCount), m_maxParticleCount(maxParticleCount),
			m_minLifetime(minLifetime), m_maxLifetime(maxLifetime), m_acceleration(acceleration) { }

		void spawn(Vector2 position);

	};
}
