#pragma once

#include <assert.h>

namespace mgd
{
	using Scalar = float;

	struct Vector3
	{
		Scalar x, y, z;

		//Constructors
		Vector3() : x(0), y(0), z(0) {}
		Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}

		//Operator overloads
		Vector3 operator + (const Vector3& v) const { return Vector3(x + v.x, y + v.y, z + v.z); }
		void operator += (const Vector3& v) { x += v.x; y += v.y; z += v.z; }

		Vector3 operator - (const Vector3& v) const { return Vector3(x - v.x, y - v.y, z - v.z); }
		void operator -= (const Vector3& v) { x -= v.x; y -= v.y; z -= v.z; }

		Vector3 operator * (Scalar k) const { return Vector3(x * k, y * k, z * k); }
		void operator *= (Scalar k) { x *= k; y *= k; z *= k; }

		Vector3 operator / (Scalar k) const { return Vector3(x / k, y / k, z / k); }
		void operator /= (Scalar k) { x /= k; y /= k; z /= k; }

		Vector3 operator - () const { return Vector3(-x, -y, -z); }
		Vector3 operator + () const { return Vector3(x, y, z); }

		//Access to individual coordinates as a vector
		Scalar operator [] (int i) const 
		{
			switch(i)
			{
			case 0:
				return x;
			case 1:
				return y;
			case 2:
				return z;
			default:
				assert(0); return 0;
			}
		}

		Scalar& operator [] (int i)
		{
			static Scalar dummy;
			switch (i)
			{
			case 0:
				return x;
			case 1:
				return y;
			case 2:
				return z;
			default:
				assert(0); return dummy;
			}
		}
	};

	inline Scalar dot(const Vector3& v, const Vector3& w) { return v.x * w.x + v.y * w.y + v.z * w.z; }

	inline Vector3 cross(const Vector3& v, const Vector3& w) 
	{
		return Vector3(
			(v.y * w.z) - (w.y * v.z),
			(v.z * w.x) - (w.z * v.x),
			(v.x * w.y) - (w.x * v.y)
		);
	}

}; //End of namespace mgd