#include "Helicopter.h"

Helicopter::Helicopter()
	: GameActor()
{
}

Helicopter::~Helicopter()
{
}

void Helicopter::load(const std::shared_ptr<rgl::ObjectParams> pObjectParams)
{
	GameActor::load(pObjectParams);

	m_acceleration.setX(0.1);
}

void Helicopter::update()
{
	GameActor::update();
}

void Helicopter::draw()
{
	GameActor::draw();
}

void Helicopter::clean()
{
}