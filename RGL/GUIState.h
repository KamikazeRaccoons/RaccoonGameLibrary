#pragma once

#include "stdafx.h"
#include "GameState.h"

#ifdef RGL_EXPORTS
#define RGL_GUISTATE_API __declspec(dllexport)
#else
#define RGL_GUISTATE_API __declspec(dllimport)
#endif

namespace rgl
{
	class GUIState : public GameState
	{
	protected:

		typedef void(*Callback)();

		std::vector<Callback> m_callbacks;

		RGL_GUISTATE_API void updateCallbacks();
	};
}