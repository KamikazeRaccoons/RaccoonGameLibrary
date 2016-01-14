#pragma once

#include "stdafx.h"
#include "GameState.h"

#ifdef RGL_EXPORTS
#define RGL_GAMESTATEMACHINE_API __declspec(dllexport)
#else
#define RGL_GAMESTATEMACHINE_API __declspec(dllimport)
#endif

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

		RGL_GAMESTATEMACHINE_API void pollTransitions();

	public:

		GameStateMachine() { }
		~GameStateMachine() { }

		RGL_GAMESTATEMACHINE_API void pushState(std::shared_ptr<GameState> pGameState);
		RGL_GAMESTATEMACHINE_API void changeState(std::shared_ptr<GameState> pGameState);
		RGL_GAMESTATEMACHINE_API void popState();

		RGL_GAMESTATEMACHINE_API void update();
		RGL_GAMESTATEMACHINE_API void render();
		RGL_GAMESTATEMACHINE_API void clean();
	};
}