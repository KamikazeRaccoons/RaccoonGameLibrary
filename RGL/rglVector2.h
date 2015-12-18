#pragma once

#include "stdafx.h"

#ifdef RGL_EXPORTS
#define RGL_VECTOR2_API __declspec(dllexport)
#else
#define RGL_VECTOR2_API __declspec(dllimport)
#endif

class rglVector2
{
private:

	double m_x;
	double m_y;

public:

	rglVector2() : m_x(0.0), m_y(0.0) { };
	rglVector2(double x, double y) : m_x(x), m_y(y) { }

	RGL_VECTOR2_API void setX(double x);
	RGL_VECTOR2_API void setY(double y);

	RGL_VECTOR2_API double getX();
	RGL_VECTOR2_API double getY();

	RGL_VECTOR2_API double getLength();
	
	RGL_VECTOR2_API rglVector2 operator+(const rglVector2& v2) const;
	RGL_VECTOR2_API rglVector2& operator+=(const rglVector2& v2);

	RGL_VECTOR2_API rglVector2 operator-(const rglVector2& v2) const;
	RGL_VECTOR2_API rglVector2& operator-=(const rglVector2& v2);

	RGL_VECTOR2_API rglVector2 operator*(double scalar) const;
	RGL_VECTOR2_API rglVector2& operator*=(double scalar);

	RGL_VECTOR2_API rglVector2 operator/(double scalar) const;
	RGL_VECTOR2_API rglVector2& operator/=(double scalar);

	RGL_VECTOR2_API void normalize();

};