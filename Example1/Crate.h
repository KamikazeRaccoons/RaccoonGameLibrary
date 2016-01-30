#pragma once

#include <RGL.h>

class Crate : public rgl::PhysicsObject
{
public:

	Crate(int x, int y, int width, int height, std::string textureID, std::string name)
		: rgl::PhysicsObject(x, y, width, height, b2_dynamicBody, textureID, name) { }
	
	virtual void onCreate();

	virtual void update();

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
