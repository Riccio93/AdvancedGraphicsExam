#pragma once

#include "vector3.h"

namespace mgd
{
	//Shapes classes

	struct Ray
	{
		Point3 p;
		Versor3 d;

		Ray(Point3 _p, Vector3 _d) : p(_p), d(_d.normalized()){	}
	};

	struct Sphere
	{
		Point3 c;
		Scalar r;

		Sphere(Point3 _c, Scalar _r) : c(_c), r(_r) {}
	};

	struct Plane
	{
		Versor3 n;
		Point3 p;
	};

	//Raycasts

	bool hit(Ray ray, Plane plane, Point3& hitPos)
	{
		Scalar dn = dot(ray.d, plane.n);
		if (dn == 0)
			return false;
		Scalar k = dot(ray.p - plane.p, plane.n) / dn; 
		if (k < 0)
			return false;
		hitPos = ray.p + k * ray.d;
		return true;
	}

	bool hit(Ray ray, Sphere sphere, Point3 &hitPos)
	{
		//hitpos is (ray.p + k*ray.d)
		//with a * k^2 + b * k + c = 0
		Scalar a = 1.f;
		Scalar b = 2 * dot(ray.d, ray.p - sphere.c);
		Scalar c = (ray.p - sphere.c).squaredNorm() - sphere.r * sphere.r;

		Scalar delta = b * b - 4 * a * c;
		if (delta < 0)
			return false;
		Scalar k = (-b - sqrt(delta)) / (2 * a);
		if (k < 0)
			return false;
		hitPos = ray.p + k * ray.d;
		return true;
	}

} //End of mgd namespace

