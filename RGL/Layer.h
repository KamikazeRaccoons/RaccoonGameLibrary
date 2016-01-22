#pragma once

#include "stdafx.h"

namespace rgl
{
	class Layer
	{
	protected:

		virtual ~Layer() { }

	public:

		virtual void update() = 0;
		virtual void render() = 0;
		virtual void clean() = 0;

	};
}