#pragma once

#include "SimpleActor.h"

namespace rgl
{
	class Particle : public SimpleActor
	{
	private:

		int m_lifetime;
		int m_lifeLeft;

	public:

		Particle(int x, int y, int width, int height, float angle,
			rgl::Vector2 velocity, rgl::Vector2 acceleration, float angularVelocity,
			int lifetime, std::string textureID, std::string name = "(unnamed Particle)");

		virtual void update();
		virtual void draw();

	};

}