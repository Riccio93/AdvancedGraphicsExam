#include <iostream>
#include "vector3.h"
#include "shapes3d.h"
#include "camera.h"

using namespace mgd;

void examplesOfSyntax()
{
	Vector3 v(0, 2, 3);
	Vector3 r = v * 4;
	r *= 0.25f;
	Vector3 w = v + r;
	v += w;
	v = -w;
	Scalar k = dot(v, w);
	Scalar q = v[0];
}

void unitTestLinearOps()
{
	Vector3 a(24, 1, 32), b(12, 3, 54);
	Scalar k = 5.f;

	assert(areEqual(a + b, b + a));	
	assert(areEqual((a + b) * k, b * k + a * k));
}

void unitTestProducts()
{
	Vector3 a(24, -3, 32), b(12, 3, -54), c(10, 13, -43);
	Scalar k = .4f;

	assert(isZero(dot(cross(a, b), a)));
	assert(isZero(dot(cross(a, b), b)));
	assert(areEqual(dot(k*a, b), dot(b,k*a)));
	assert(areEqual(cross(a, b + c), cross(a,b) + cross(a, c)));
}

void unitTestRaycasts()
{
	Ray r(Point3(0, 0, 0), Vector3(1, 0, 0));
	Sphere s(Point3(5, 0, 0), 3);

	Point3 hitPos;
	Versor3 hitNorm;
	Scalar distMax = 100000;
	bool isHit = rayCast(r, s, hitPos, hitNorm, distMax);
	assert(isHit);
	assert(areEqual(hitPos, Point3(2, 0, 0)));
}

//ASCII art: convert an intensity value [0,1] into a sequence of two chars
const char* intensityToCstr(Scalar intensity)
{
	switch(int (std::round(intensity*5)))
	{
		case 0: return " "; //darkest
		case 1: return " \'";
		case 2: return " +";
		case 3: return " *";
		case 4: return " #";
		default: case 5: return "##"; //lightest
	}
}

const char* lighting(Versor3 normal)
{
	//lambertian lighting (dot prod between normal and light's direction, max when dot = 1 (most similar, fully in light)
	const Versor3 lightDir = Versor3(1, 2, -2).normalized(); 
	Scalar diffuse = dot(normal, lightDir);
	if (diffuse < 0) diffuse = 0;
	return intensityToCstr(diffuse);
}

void rayCastingSphere()
{
	Camera c(2.f, 30, 30);
	Sphere sphere(Point3(0, 0, 6), 2);
	Plane plane(Point3(0, .5f, 0), Versor3(0, 1, 0));

	for(int y = 0; y < c.pixelDimY; y++)
	{
		for(int x = 0; x < c.pixelDimX; x++)
		{
			Point3 hitPos;
			Versor3 hitNorm;
			Scalar distMax = 100000.f;
			rayCast(c.primaryRay(x, y), sphere, hitPos, hitNorm, distMax);
			rayCast(c.primaryRay(x, y), plane, hitPos, hitNorm, distMax);
			std::cout << lighting(hitNorm);
		}
		std::cout << std::endl;
	}
}

int main()
{
	unitTestProducts();
	unitTestLinearOps();
	unitTestRaycasts();
	rayCastingSphere();
}