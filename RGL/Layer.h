#pragma once

#include "stdafx.h"

#ifdef RGL_EXPORTS
#define RGL_LAYER_API __declspec(dllexport)
#else
#define RGL_LAYER_API __declspec(dllimport)
#endif

namespace rgl
{
	class Layer
	{
	protected:

		virtual ~Layer() { }

	public:

		virtual void update() = 0;
		virtual void render() = 0;

	};
}