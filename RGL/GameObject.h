#pragma once

#include "stdafx.h"
#include "ObjectParams.h"

#ifdef RGL_EXPORTS
#define RGL_GAMEOBJECT_API __declspec(dllexport)
#else
#define RGL_GAMEOBJECT_API __declspec(dllimport)
#endif

namespace rgl
{
	class GameObject
	{
	protected:

		GameObject() { }

	public:

		virtual void load(const std::shared_ptr<ObjectParams> pObjectParams) = 0;

		virtual void update() = 0;
		virtual void draw() = 0;
		virtual void clean() = 0;

		virtual ~GameObject() { }

	};
}