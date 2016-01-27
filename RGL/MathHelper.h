#pragma once

#include "stdafx.h"

namespace rgl
{
	class MathHelper
	{
	public:

		static float toDeg(float rad) { return rad * (180.0f / (float)3.14); }
		static float toRad(float deg) { return deg * ((float)3.14 / 180.0f); }

	};
}