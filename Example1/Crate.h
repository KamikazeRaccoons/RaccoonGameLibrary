#pragma once

#include <RGL.h>

class Crate : public rgl::PhysicsObject
{
private:

	rgl::Vector2 m_contactPosition;

	bool m_colliding;

public:

	Crate(int x, int y, int width, int height, std::string textureID, std::string name)
		: rgl::PhysicsObject(x, y, width, height, b2_dynamicBody, textureID, name), m_colliding(false) { }
	
	virtual void onCreate();
	virtual void update();
	virtual void draw();
	virtual void onBeginContact(rgl::Vector2 contactPosition, rgl::PhysicsObject* pPhysicsObject);
	virtual void onEndContact(rgl::Vector2 contactPosition, rgl::PhysicsObject* pPhysicsObject);

};

class CrateCreator : public rgl::ObjectCreator
{
	virtual std::shared_ptr<rgl::GameObject> createObject(const std::shared_ptr<rgl::ObjectParams> pObjectParams, std::string name) const
	{
		return std::make_shared<Crate>(pObjectParams->getIntParam("x"), pObjectParams->getIntParam("y"),
			pObjectParams->getIntParam("width"), pObjectParams->getIntParam("height"),
			pObjectParams->getStringParam("textureID"), name);
	}
};
