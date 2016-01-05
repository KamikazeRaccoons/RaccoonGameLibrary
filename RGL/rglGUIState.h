#pragma once

#include "stdafx.h"
#include "rglGameState.h"

#ifdef RGL_EXPORTS
#define RGL_GUISTATE_API __declspec(dllexport)
#else
#define RGL_GUISTATE_API __declspec(dllimport)
#endif

class rglGUIState : public rglGameState
{
protected:

	typedef void(*Callback)();

	vector<Callback> m_callbacks; 

	RGL_GUISTATE_API void updateCallbacks();
};