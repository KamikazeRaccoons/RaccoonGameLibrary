#pragma once

#include <memory>
#include <string>

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

	public:

		PhysicsObject(int x, int y, int width, int height, b2BodyType bodyType, std::string textureID, std::string name)
			: GameObject(name), m_x(x), m_y(y), m_width(width), m_height(height), m_bodyType(bodyType), m_textureID(textureID) { }

		virtual void onCreate();
		virtual void onDestroy();

		virtual void update();
		virtual void draw();

		virtual int getX();
		virtual int getY();

		virtual int getWidth();
		virtual int getHeight();

	};

	class PhysicsObjectCreator : public ObjectCreator
	{
		virtual std::shared_ptr<GameObject> createObject(const std::shared_ptr<ObjectParams> pObjectParams, std::string name) const
		{
			b2BodyType bodyType = b2_staticBody;
			std::string bodyTypeParam = pObjectParams->getStringParam("bodyType");

			if (bodyTypeParam == "dynamicBody")
				bodyType = b2_dynamicBody;

			return std::make_shared<PhysicsObject>(pObjectParams->getIntParam("x"), pObjectParams->getIntParam("y"),
				pObjectParams->getIntParam("width"), pObjectParams->getIntParam("height"), bodyType, pObjectParams->getStringParam("textureID"), name);
		}
	};
}