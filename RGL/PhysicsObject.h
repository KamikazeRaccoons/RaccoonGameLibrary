#pragma once

#include "stdafx.h"
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

		b2BodyDef m_bodyDef;
		b2Body* m_pBody;

		std::vector<b2Fixture*> m_fixtures;

		std::string m_textureID;

		RGL_API void addFixture(b2FixtureDef* pFixtureDef);

	public:

		PhysicsObject(int x, int y, int width, int height, std::string textureID, std::string name = "(unnamed PhysicsObject)")
			: GameObject(name), m_x(x), m_y(y), m_width(width), m_height(height), m_textureID(textureID) { }

		virtual RGL_API void onCreate();
		virtual RGL_API void onDestroy();

		virtual RGL_API void update();
		virtual RGL_API void draw();

	};

	class PhysicsObjectCreator : public ObjectCreator
	{
		virtual std::shared_ptr<GameObject> createObject(const std::shared_ptr<ObjectParams> pObjectParams, std::string name) const
		{
			return std::make_shared<PhysicsObject>(pObjectParams->getIntParam("x"), pObjectParams->getIntParam("y"),
				pObjectParams->getIntParam("width"), pObjectParams->getIntParam("height"), pObjectParams->getStringParam("textureID"), name);
		}
	};
}