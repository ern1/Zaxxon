#pragma once
#include <cmath>
#include "vector2D.h"

class IsoVector {

public:
	double x;
	double y;
	double z; // used for altitude

	IsoVector(double x, double y, double z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}

	IsoVector(double x, double y) {
		this->x = x;
		this->y = y;
		this->z = 0.;
	}

	IsoVector() {
		this->x = 0.;
		this->y = 0.;
		this->z = 0.;
	}

	IsoVector operator+(const IsoVector& rhs)
	{
		return IsoVector(x + rhs.x, y + rhs.y, z + rhs.z);
	}

	IsoVector operator-(const IsoVector& rhs)
	{
		return IsoVector(x - rhs.x, y - rhs.y, z - rhs.z);
	}

	IsoVector operator*(const IsoVector& rhs)
	{
		return IsoVector(x * rhs.x, y * rhs.y, z * rhs.z);
	}

	IsoVector operator/(const IsoVector& rhs)
	{
		return IsoVector(x / rhs.x, y / rhs.y, z / rhs.z);
	}

	IsoVector operator/(const double& rhs)
	{
		return IsoVector(x / rhs, y / rhs, z / rhs);
	}

	IsoVector operator*(const double& rhs)
	{
		return IsoVector(x * rhs, y * rhs, z * rhs);
	}

	void operator+=(const IsoVector& rhs)
	{
		x += rhs.x; y += rhs.y; z += rhs.z;
	}

	void operator-=(const IsoVector& rhs)
	{
		x -= rhs.x; y -= rhs.y; z -= rhs.z;
	}

	void operator*=(const double& rhs)
	{
		x *= rhs; y *= rhs; z *= rhs;
	}

	void operator/=(const double& rhs)
	{
		x /= rhs; y /= rhs; z /= rhs;
	}

	IsoVector normal()
	{
		double length = std::sqrt(x * x + y * y + z * z);

		// TODO: I don't need to check this?
		if (length != 0.) {
			return IsoVector(x /= length, y /= length, z /= length);
		}

		return IsoVector(x /= length, y /= length, z /= length);
	}

	void normalize()
	{
		double length = std::sqrt(x * x + y * y + z * z);

		if (length != 0.) {
			x /= length;
			y /= length;
			z /= length;
		}
	}

	double distance(const IsoVector& rhs)
	{
		return std::sqrt(std::pow(x - rhs.x, 2) + std::pow(y - rhs.y, 2) + std::pow(z - rhs.z, 2));
	}

	double sum()
	{
		return x + y + z;
	}

	Vector2D to2D(double inc = 0.5)
	{
		return Vector2D(x - y, (x + y) * inc - z);
	}

	static Vector2D fromIsoto2D(IsoVector vec, double inc = 0.5)
	{
		return Vector2D(vec.x - vec.y, (vec.x + vec.y) * inc - vec.z);
	}

	static IsoVector from2DtoISO(Vector2D vec, double inc = 0.5)
	{
		return IsoVector(
			(2 * vec.y + vec.x) * inc,
			(2 * vec.y - vec.x) * inc,
			0.0
		);
	}

	static IsoVector from2DtoISO(Vector2D vec, Vector2D origin, double inc = 0.5)
	{
		vec -= origin;
		return IsoVector(
			(2 * vec.y + vec.x) * inc,
			(2 * vec.y - vec.x) * inc,
			0.0
		);
	}
};
