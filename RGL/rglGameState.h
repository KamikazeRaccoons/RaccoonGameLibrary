#pragma once

#include "stdafx.h"

class rglGameState
{
public:

	virtual void update() = 0;
	virtual void render() = 0;

	virtual void onEnter() = 0;
	virtual void onExit() = 0;

	virtual string getStateID() const = 0;
};