#include "Vector2.h"

#include <cmath>

namespace rgl
{
	void Vector2::setX(double x)
	{
		m_x = x;
	}

	void Vector2::setY(double y)
	{
		m_y = y;
	}

	double Vector2::getX()
	{
		return m_x;
	}

	double Vector2::getY()
	{
		return m_y;
	}

	double Vector2::getLength()
	{
		return std::sqrt(m_x * m_x + m_y * m_y);
	}

	Vector2 Vector2::operator+(const Vector2& v2) const
	{
		return Vector2(m_x + v2.m_x, m_y + v2.m_y);
	}

	Vector2& Vector2::operator+=(const Vector2& v2)
	{
		m_x += v2.m_x;
		m_y += v2.m_y;

		return *this;
	}

	Vector2 Vector2::operator-(const Vector2& v2) const
	{
		return Vector2(m_x - v2.m_x, m_y - v2.m_y);
	}

	Vector2& Vector2::operator-=(const Vector2& v2)
	{
		m_x -= v2.m_x;
		m_y -= v2.m_y;

		return *this;
	}

	Vector2 Vector2::operator*(double scalar) const
	{
		return Vector2(m_x * scalar, m_y * scalar);
	}

	Vector2& Vector2::operator*=(double scalar)
	{
		m_x *= scalar;
		m_y *= scalar;

		return *this;
	}

	Vector2 Vector2::operator/(double scalar) const
	{
		return Vector2(m_x / scalar, m_y / scalar);
	}

	Vector2& Vector2::operator/=(double scalar)
	{
		m_x /= scalar;
		m_y /= scalar;

		return *this;
	}

	void Vector2::normalize()
	{
		double l = getLength();

		if (l > 0)
			*this *= 1.0 / l;
	}
}