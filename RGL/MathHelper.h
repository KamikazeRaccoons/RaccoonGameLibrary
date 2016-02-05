#pragma once

namespace rgl
{
	class MathHelper
	{
	public:

		static float toDeg(float rad) { return rad * (180.0f / (float)3.14); }
		static float toRad(float deg) { return deg * ((float)3.14 / 180.0f); }

		static int randInt(int min, int max)
		{
			return rand() % (max - min + 1) + min;
		}

		static float randFloat(float min, float max)
		{
			float r = (float)rand() / RAND_MAX;
			return min + r * (max - min);
		}

	};
}