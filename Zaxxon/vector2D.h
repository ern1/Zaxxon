#pragma once
#include <cmath>

class Vector2D {
public:
	double x;
	double y;

	Vector2D operator+(const Vector2D& rhs)
	{
		return Vector2D(x + rhs.x, y + rhs.y);
	}

	Vector2D operator-(const Vector2D& rhs)
	{
		return Vector2D(x - rhs.x, y - rhs.y);
	}

	Vector2D operator*(const Vector2D& rhs)
	{
		return Vector2D(x * rhs.x, y * rhs.y);
	}

	Vector2D operator/(const double& rhs)
	{
		return Vector2D(x / rhs, y / rhs);
	}

	Vector2D operator*(const double& rhs)
	{
		return Vector2D(x * rhs, y * rhs);
	}

	void operator+=(const Vector2D& rhs)
	{
		x += rhs.x; y += rhs.y;
	}

	void operator-=(const Vector2D& rhs)
	{
		x -= rhs.x; y -= rhs.y;
	}

	double dotProduct(const Vector2D& rhs)
	{
		return x * rhs.x + y * rhs.y;
	}

	// 
	double distance(const Vector2D& rhs)
	{
		return std::sqrt(std::pow(x - rhs.x, 2) + std::pow(y - rhs.y, 2));
	}

	Vector2D(double x, double y) {
		this->x = x;
		this->y = y;
	}
	Vector2D() {
		this->x = 0;
		this->y = 0;
	}
};
