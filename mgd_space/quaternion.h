#pragma once

#ifndef M_PI
	#define M_PI 3.14159265358979323846f
#endif

#include "vector3.h"

namespace mgd
{
	class Quaternion
	{

	public:

		Vector3 im;
		Scalar re;	

		//Constructors
		Quaternion(const Vector3& _im, Scalar _re) : im(_im), re(_re) {}

		//Conjugate
		void conjugate() { im = -im; }
		Quaternion conjugated() const { return Quaternion(-im, re);	}

		static Quaternion fromVector3(const Vector3& v)
		{
			return Quaternion(v, 0);
		}

		static Quaternion fromAngleAxis(Scalar deg, Vector3 axis)
		{
			Scalar rad = deg * M_PI / 180.f;
			return Quaternion
			(
				axis.normalized() * std::sin(rad / 2),
				std::cos(rad / 2)
			);
		}

		static Quaternion identity(){ return Quaternion(Vector3(0,0,0), 1);	}

		//Apply
		Vector3 apply(Vector3 p) const;

		inline Quaternion operator - () const { return Quaternion(-im, -re); }
	};

	inline Quaternion operator * (const Quaternion& a, const Quaternion& b)
	{
		return Quaternion(
			cross(a.im, b.im) + a.im * b.re + b.im * a.re,
			-dot(a.im, b.im) + a.re * b.re
		);
	}


	inline Vector3 Quaternion::apply(Vector3 p) const
	{
		Quaternion q = fromVector3(p);
		q = (*this) * q * this->conjugated();
		return q.im;
	}

	inline bool areEqual(const Quaternion& a, const Quaternion& b)
	{
		return areEqual(a.im, b.im) && areEqual(a.re, b.re);
	}

	inline bool areEquivalent(const Quaternion& a, const Quaternion& b)
	{
		return areEqual(a, b) || areEqual(a, -b);
	}
}

