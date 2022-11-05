#pragma once

#include <cassert>
#include <cmath>

#define TOLERANCE 1e-5;

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

		bool operator == (const Vector3 &v) { return (x == v.x) && (y == v.y) && (z == v.z); }

		//norm stuff
		Scalar squaredNorm() const { return x * x + y * y + z * z; }
		Scalar norm() const { return std::sqrt(squaredNorm()); }
		Vector3 normalized() const { return *this / norm(); }
		void normalize() { *this /= norm(); }
	};

	using Point3 = Vector3; //Useful if we'll want to make a separate Point3 struct later
	using Versor3 = Vector3; //Useful if we'll want to make a separate Versor3 struct later

	//Necessary because vector scaling is commutative
	inline Vector3 operator * (Scalar k, const Vector3 &a) { return a * k; }

	//Vector operations
	inline Scalar dot(const Vector3& v, const Vector3& w) { return v.x * w.x + v.y * w.y + v.z * w.z; }

	inline Vector3 cross(const Vector3& v, const Vector3& w)
	{
		return Vector3(
			(v.y * w.z) - (w.y * v.z),
			(v.z * w.x) - (w.z * v.x),
			(v.x * w.y) - (w.x * v.y)
		);
	}

	//Support functions
	inline bool areEqual(Scalar a, Scalar b) { return std::abs(a - b) < TOLERANCE; }

	inline bool areEqual(const Vector3 &a, const Vector3 &b) 
	{
		return areEqual(a.x, b.x) && areEqual(a.y, b.y) && areEqual(a.z, b.z);
	}	

	inline bool isZero(Scalar a) { return std::abs(a) < TOLERANCE; }

	inline bool isZero(Vector3 a) { return isZero(a.x) && isZero(a.y) && isZero(a.z); }

}; //End of namespace mgd