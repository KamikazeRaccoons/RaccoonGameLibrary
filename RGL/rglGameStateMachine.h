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

	std::vector<rglGameState*> m_gameStates;

public:

	void RGL_GAMESTATEMACHINE_API pushState(rglGameState* pGameState);
	void RGL_GAMESTATEMACHINE_API changeState(rglGameState* pGameState);
	void RGL_GAMESTATEMACHINE_API popState();

	void RGL_GAMESTATEMACHINE_API update();
	void RGL_GAMESTATEMACHINE_API render();
};