#pragma once

#include "stdafx.h"

namespace rgl
{
	class Vector2
	{
	private:

		double m_x;
		double m_y;

	public:

		Vector2() : m_x(0.0), m_y(0.0) { };
		Vector2(double x, double y) : m_x(x), m_y(y) { }

		RGL_API void setX(double x);
		RGL_API void setY(double y);

		RGL_API double getX();
		RGL_API double getY();

		RGL_API double getLength();

		RGL_API Vector2 operator+(const Vector2& v2) const;
		RGL_API Vector2& operator+=(const Vector2& v2);

		RGL_API Vector2 operator-(const Vector2& v2) const;
		RGL_API Vector2& operator-=(const Vector2& v2);

		RGL_API Vector2 operator*(double scalar) const;
		RGL_API Vector2& operator*=(double scalar);

		RGL_API Vector2 operator/(double scalar) const;
		RGL_API Vector2& operator/=(double scalar);

		RGL_API void normalize();

	};
}