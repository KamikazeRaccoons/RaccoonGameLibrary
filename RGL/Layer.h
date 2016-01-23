#pragma once

#include "stdafx.h"
#include "Level.h"

namespace rgl
{
	class Layer
	{
	protected:

		std::shared_ptr<Level> m_pLevel;

		Layer(std::shared_ptr<Level> pLevel) : m_pLevel(pLevel) { }
		virtual ~Layer() { }

	public:

		virtual void update() = 0;
		virtual void render() = 0;
		virtual void clean() = 0;

	};
}