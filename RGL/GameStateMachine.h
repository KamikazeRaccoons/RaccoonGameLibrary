#pragma once

#include "stdafx.h"
#include "GameState.h"

namespace rgl
{
	class GameStateMachine
	{
	private:

		enum TransitionType
		{
			PUSH = 0,
			CHANGE = 1,
			POP = 2
		};

		std::vector<std::shared_ptr<GameState>> m_gameStates;
		std::vector<std::pair<TransitionType, std::shared_ptr<GameState>>> m_queuedTransitions;

		RGL_API void pollTransitions();

	public:

		GameStateMachine() { }
		~GameStateMachine() { }

		RGL_API void pushState(std::shared_ptr<GameState> pGameState);
		RGL_API void changeState(std::shared_ptr<GameState> pGameState);
		RGL_API void popState();

		RGL_API void update();
		RGL_API void render();
		RGL_API void clean();
	};
}