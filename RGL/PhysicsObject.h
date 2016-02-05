#pragma once

#include <memory>
#include <string>
#include <vector>

#include <Box2D\Box2D.h>

#include "GameObject.h"
#include "ObjectFactory.h"

namespace rgl
{
	class PhysicsObject : public GameObject
	{
	protected:

		int m_x;
		int m_y;

		int m_width;
		int m_height;

		b2BodyType m_bodyType;
		b2Body* m_pBody;

		std::string m_textureID;

		std::vector<b2Fixture*> m_fixtures;

		void addFixture(b2FixtureDef* pFixtureDef);

	public:

		PhysicsObject(int x, int y, int width, int height, b2BodyType bodyType, std::string textureID, std::string name = "(unnamed PhysicsObject)")
			: GameObject(name), m_x(x), m_y(y), m_width(width), m_height(height), m_bodyType(bodyType), m_textureID(textureID) { }

		virtual void onCreate();
		virtual void onDestroy();

		virtual void update();
		virtual void draw();

		virtual int getX();
		virtual int getY();

		virtual int getWidth();
		virtual int getHeight();

		virtual void onBeginContact(Vector2 contactPosition, PhysicsObject* pPhysicsObject) { };
		virtual void onEndContact(Vector2 contactPosition, PhysicsObject* pPhysicsObject) { }

	};
}