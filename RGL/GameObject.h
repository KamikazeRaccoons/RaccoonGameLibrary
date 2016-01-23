#pragma once

#include "stdafx.h"
#include "ObjectParams.h"
#include "Vector2.h"

namespace rgl
{
	class Level;

	class GameObject
	{
	protected:

		std::shared_ptr<Level> m_pLevel;

	public:

		virtual void onCreate() = 0;
		virtual void onDestroy() = 0;

		virtual void update() = 0;
		virtual void draw() = 0;

		RGL_API void setParentLevel(std::shared_ptr<Level> pParentLevel);

	};
}