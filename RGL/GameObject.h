#pragma once

#include "stdafx.h"
#include "ObjectParams.h"
#include "Vector2.h"

namespace rgl
{
	class Level;

	class GameObject : public std::enable_shared_from_this<GameObject>
	{
	protected:

		std::shared_ptr<Level> m_pLevel;
		std::string m_name;

	public:

		GameObject(std::string name = "(unnamed)") : m_name(name) { }

		virtual void onCreate() = 0;
		virtual void onDestroy() = 0;

		virtual void update() = 0;
		virtual void draw() = 0;

		RGL_API void setParentLevel(std::shared_ptr<Level> pParentLevel);
		RGL_API void setName(std::string name);

	};
}