#pragma once

#include "GameObject.h"

namespace rgl
{
	class GameState
	{
	public:

		virtual void update() = 0;
		virtual void render() = 0;

		virtual void onEnter() = 0;
		virtual void onExit() = 0;

		virtual std::string getStateID() const = 0;
	};
}