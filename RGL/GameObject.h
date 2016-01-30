#pragma once

#include <memory>
#include <string>

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

		GameObject(std::string name) : m_name(name) { }

		virtual void onCreate() = 0;
		virtual void onDestroy() = 0;

		virtual void update() = 0;
		virtual void draw() = 0;

		virtual int getX() = 0;
		virtual int getY() = 0;

		virtual int getWidth() = 0;
		virtual int getHeight() = 0;

		void setParentLevel(std::shared_ptr<Level> pParentLevel);
		void setName(std::string name);

		int getDiameter();

		void debugDrawName();

	};
}