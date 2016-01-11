#pragma once

#include "stdafx.h"
#include "GameObject.h"

namespace rgl
{
	class GameState
	{
	protected:

		std::vector<std::shared_ptr<GameObject>> m_gameObjects;
		std::vector<std::string> m_textureIDs;

	public:

		virtual void update() = 0;
		virtual void render() = 0;

		virtual void onEnter() = 0;
		virtual void onExit() = 0;

		virtual std::string getStateID() const = 0;
	};
}