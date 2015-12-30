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

	enum TransitionType
	{
		PUSH = 0,
		CHANGE = 1,
		POP = 2
	};

	vector<shared_ptr<rglGameState>> m_gameStates;
	vector<pair<TransitionType, shared_ptr<rglGameState>>> m_queuedTransitions;

public:

	rglGameStateMachine() { }
	~rglGameStateMachine() { }

	RGL_GAMESTATEMACHINE_API void pushState(shared_ptr<rglGameState> pGameState);
	RGL_GAMESTATEMACHINE_API void changeState(shared_ptr<rglGameState> pGameState);
	RGL_GAMESTATEMACHINE_API void popState();
	
	RGL_GAMESTATEMACHINE_API void update();
	RGL_GAMESTATEMACHINE_API void render();
};