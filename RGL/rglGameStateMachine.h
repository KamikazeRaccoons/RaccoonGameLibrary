#pragma once

#include "stdafx.h"
#include "rglGameState.h"

#ifdef RGL_EXPORTS
#define RGL_GAMESTATEMACHINE_API __declspec(dllexport)
#else
#define RGL_GAMESTATEMACHINE_API __declspec(dllimport)
#endif

class rglGameStateMachine
{
private:

	enum GameStateStatus
	{
		ACTIVE = 0,
		ENTERING = 1,
		EXITING = 2
	};

	vector<pair<GameStateStatus, shared_ptr<rglGameState>>> m_gameStates;

public:

	rglGameStateMachine() { }
	~rglGameStateMachine() { };

	void RGL_GAMESTATEMACHINE_API pushState(shared_ptr<rglGameState> pGameState);
	void RGL_GAMESTATEMACHINE_API changeState(shared_ptr<rglGameState> pGameState);
	void RGL_GAMESTATEMACHINE_API popState();

	void RGL_GAMESTATEMACHINE_API update();
	void RGL_GAMESTATEMACHINE_API render();
};