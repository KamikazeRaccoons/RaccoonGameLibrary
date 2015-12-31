#pragma once

#include "stdafx.h"
#include "rglGameObject.h"

class rglGameState
{
protected:

	vector<shared_ptr<rglGameObject>> m_gameObjects;
	vector<string> m_textureIDs;

public:

	virtual void update() = 0;
	virtual void render() = 0;

	virtual void onEnter() = 0;
	virtual void onExit() = 0;

	virtual string getStateID() const = 0;
};