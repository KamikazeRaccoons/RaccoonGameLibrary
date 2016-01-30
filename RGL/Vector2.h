#pragma once

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

		void setX(double x);
		void setY(double y);

		double getX();
		double getY();

		double getLength();

		Vector2 operator+(const Vector2& v2) const;
		Vector2& operator+=(const Vector2& v2);

		Vector2 operator-(const Vector2& v2) const;
		Vector2& operator-=(const Vector2& v2);

		Vector2 operator*(double scalar) const;
		Vector2& operator*=(double scalar);

		Vector2 operator/(double scalar) const;
		Vector2& operator/=(double scalar);

		void normalize();

	};
}