#ifndef __FE_VECTOR3D_H__
#define __FE_VECTOR3D_H__

#include "include/FEType.h"

using namespace FunnyEnging;

namespace FunnyEngine::Math
{
	class FEVector3D
	{
	private:
		real _x, _y, _z, _w;

	public:
		FEVector3D(real x, real y, real z, real w);
		FEVector3D(real x, real y, real z);
		FEVector3D();


		inline FEVector3D& zero()
		{
			return FEVector3D();
		}

		inline FEVector3D& xAxis()
		{
			static FEVector3D s_xAxis(1, 0, 0);
			return s_xAxis;
		}

		inline FEVector3D& yAxis()
		{
			static FEVector3D s_yAxis(0, 1, 0);
			return s_yAxis;
		}

		inline FEVector3D& zAxis()
		{
			static FEVector3D s_zAxis(0, 0, 1);
			return s_zAxis;
		}

		inline FEVector3D& left()
		{
			static FEVector3D s_left(-1, 0, 0);
			return s_left;
		}

		inline FEVector3D& right()
		{
			return xAxis();
		}

		inline FEVector3D& up()
		{
			return yAxis();
		}

		inline FEVector3D& down()
		{
			static FEVector3D s_down(0, -1, 0);
			return s_down;
		}

		inline FEVector3D& forward()
		{
			static FEVector3D s_forward(0, 0, -1);
			return s_forward;
		}

		inline FEVector3D& backward()
		{
			return zAxis();
		}

		real magnitude() const;
		real magnitudeSquared() const;

		real distanceTo(const FEVector3D& other) const;

		real dot(const FEVector3D& other);

		FEVector3D& cross(const FEVector3D& other);

		FEVector3D& normalized();
	};
}
#endif