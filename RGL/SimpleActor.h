#pragma once

#include <string>

#include "GameObject.h"

namespace rgl
{
	class SimpleActor : public GameObject
	{
	protected:

		Vector2 m_position;
		Vector2 m_velocity;
		Vector2 m_acceleration;

		int m_width;
		int m_height;

		float m_angle;
		float m_angularVelocity;

		std::string m_textureID;

	public:
		
		SimpleActor(int x, int y, int width, int height, float angle, std::string textureID, std::string name = "(unnamed SimpleActor)")
			: GameObject(name), m_position(x, y), m_width(width), m_height(height), m_angle(angle), m_textureID(textureID) { }

		virtual void update();
		virtual void draw();

		virtual int getX() { return (int)m_position.getX(); }
		virtual int getY() { return (int)m_position.getY(); }

		virtual int getWidth() { return m_width; }
		virtual int getHeight() { return m_height; }

	};
}
