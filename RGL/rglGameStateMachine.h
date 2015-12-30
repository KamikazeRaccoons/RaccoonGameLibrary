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

	bool m_polling;

	RGL_GAMESTATEMACHINE_API void pollStateChanges();

public:

	rglGameStateMachine() : m_polling(false) { }
	~rglGameStateMachine() { };

	RGL_GAMESTATEMACHINE_API void pushState(shared_ptr<rglGameState> pGameState);
	RGL_GAMESTATEMACHINE_API void changeState(shared_ptr<rglGameState> pGameState);
	RGL_GAMESTATEMACHINE_API void popState();
	
	RGL_GAMESTATEMACHINE_API void update();
	RGL_GAMESTATEMACHINE_API void render();
};