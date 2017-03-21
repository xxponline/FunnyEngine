#include "Include/FEVector3D.h"
#include "Include/FEConstant.h"
#include <cmath>

namespace FunnyEngine
{
	namespace Math
	{
		FEVector3D::FEVector3D(real _x, real _y, real _z)
			: x(_x), y(_y), z(_z), w(1)
		{
		}

		FEVector3D::FEVector3D()
			: x(0), y(0), z(0), w(1)
		{
		}

		real FEVector3D::magnitude() const
		{
			return std::sqrt(x*x + y*y + z*z);
		}

		real FEVector3D::magnitudeSquared() const
		{
			return x*x + y*y + z*z;
		}

		real FEVector3D::distanceTo(const FEVector3D & other) const
		{
			return std::sqrt(
				(x - other.x) * (x - other.x) +
				(y - other.y) * (y - other.y) +
				(z - other.z) * (z - other.z)
			);
		}

		real FEVector3D::dot(const FEVector3D & other)
		{
			return x * other.x + y * other.y + z + other.z;
		}

		FEVector3D FEVector3D::cross(const FEVector3D & other)
		{
			return FEVector3D((y * other.z) - (z * other.y),
				(z * other.x) - (x * other.z),
				(x * other.y) - (y * other.x));
		}

		FEVector3D FEVector3D::normalized()
		{
			real length = magnitude();
			if (length < EPSILON)
			{
				return zero();
			}
			return FEVector3D(x / length, y / length, z / length);
		}

		FEVector3D FEVector3D::operator+(FEVector3D & other)
		{
			return FEVector3D(x + other.x, y + other.y, z + other.z);
		}

		FEVector3D FEVector3D::operator-(FEVector3D & other)
		{
			return FEVector3D(x - other.x, y - other.y, z - other.z);
		}

		FEVector3D FEVector3D::operator*(real ratio)
		{
			return FEVector3D(x * ratio, y * ratio, z * ratio);
		}

		void FEVector3D::operator+=(FEVector3D & other)
		{
			x += other.x;
			y += other.y;
			z += other.z;
		}

		void FEVector3D::operator-=(FEVector3D & other)
		{
			x -= other.x;
			y -= other.y;
			z -= other.z;
		}
	}
}
