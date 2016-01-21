#pragma once

#include "stdafx.h"
#include "ObjectParams.h"
#include "Vector2.h"

#ifdef RGL_EXPORTS
#define RGL_GAMEOBJECT_API __declspec(dllexport)
#else
#define RGL_GAMEOBJECT_API __declspec(dllimport)
#endif

namespace rgl
{
	class Level;

	class GameObject
	{
	protected:

		std::shared_ptr<Level> m_pLevel;

		RGL_GAMEOBJECT_API GameObject(std::shared_ptr<Level> pParentLevel);

	public:

		virtual void onCreate() = 0;
		virtual void onDestroy() = 0;

		virtual void update() = 0;
		virtual void draw() = 0;

	};
}