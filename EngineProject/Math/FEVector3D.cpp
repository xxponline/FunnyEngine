#include "Include\FEVector3D.h"
#include <cmath>

namespace FunnyEngine::Math
{
	FEVector3D::FEVector3D(real x, real y, real z, real w)
		:_x(x), _y(y), _z(z), _w(w)
	{
	}

	FEVector3D::FEVector3D(real x, real y, real z)
		: _x(x), _y(y), _z(z), _w(0)
	{
	}

	FEVector3D::FEVector3D()
		: _x(0), _y(0), _z(0), _w(0)
	{
	}

	real FEVector3D::magnitude() const
	{
		return std::sqrt(_x*_x + _y*_y + _z*_z);
	}

	real FEVector3D::magnitudeSquared() const
	{
		return _x*_x + _y*_y + _z*_z;
	}

	real FEVector3D::distanceTo(const FEVector3D & other) const
	{
		return std::sqrt(
			(_x - other._x) * (_x - other._x) +
			(_y - other._y) * (_y - other._y) +
			(_z - other._z) * (_z - other._z)
		);
	}

	real FEVector3D::dot(const FEVector3D & other)
	{
		return _x * other._x + _y * other._y + _z + other._z;
	}

	FEVector3D & FEVector3D::cross(const FEVector3D & other)
	{
		return FEVector3D((_y * other._z) - (_z * other._y),
			(_z * other._x) - (_x * other._z),
			(_x * other._y) - (_y * other._x));
	}

	FEVector3D & FEVector3D::normalized()
	{
		real length = magnitude();
		if (length < 0.0000001)
		{
			return zero();
		}
		return FEVector3D(_x / length, _y / length, _z / length);
	}
}
