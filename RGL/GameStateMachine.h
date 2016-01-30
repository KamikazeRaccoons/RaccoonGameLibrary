#pragma once

#include <vector>
#include <memory>

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

		void pollTransitions();

	public:

		GameStateMachine() { }
		~GameStateMachine() { }

		void pushState(std::shared_ptr<GameState> pGameState);
		void changeState(std::shared_ptr<GameState> pGameState);
		void popState();

		void update();
		void render();
		void clean();
	};
}