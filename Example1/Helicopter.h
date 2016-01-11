#pragma once

#include <RGL.h>

class Helicopter : public rgl::GameActor
{
public:
	Helicopter();
	~Helicopter();

	virtual void load(const std::shared_ptr<rgl::ObjectParams> pObjectParams);

	virtual void update();
	virtual void draw();
	virtual void clean();
};

class HelicopterCreator : public rgl::ObjectCreator
{
	virtual std::shared_ptr<rgl::GameObject> createObject() const
	{
		return std::make_shared<Helicopter>();
	}
};