#include <iostream>
#include "vector3.h"
#include "shapes3d.h"
#include "camera.h"
#include "quaternion.h"
#include "transform.h"

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

void unitTestRaycastPlane()
{
	Ray r(Point3(0, 0, 0), Vector3(1, 0, 0));
	Plane p(Point3(10, 0, 0), Vector3(-1, 0, 0));
	Point3 hitPos;
	Versor3 hitNorm;
	Scalar distMax = 100000;
	bool isHit = rayCast(r, p, hitPos, hitNorm, distMax);

	assert(isHit);
	assert(areEqual(hitPos, Point3(10, 0, 0)));
}

void unitTestQuaternions()
{
	{
		Quaternion rot = Quaternion::fromAngleAxis(180.f, Vector3(0, 1, 0));
		Point3 p(0, 0, 1);
		assert(areEqual(rot.apply(p), Point3(0, 0, -1)));
	}
	{
		Quaternion rot = Quaternion::fromAngleAxis(90.f, Vector3(0, 1, 0));
		Point3 p(3, 5, 6);
		assert(areEqual(rot.apply(p), Point3(6, 5, -3)));
	}
	{
		Quaternion rot = Quaternion::fromAngleAxis(30.f, Vector3(1, 1, 3));
		Point3 p(3, 5, 6), q = p;
		for(int k=0; k<12; k++) q = rot.apply(q);
		assert(areEqual(p, q));
	}
	{
		Quaternion rot = Quaternion::fromAngleAxis(20.f, Vector3(4, 3, 1));
		Quaternion q = Quaternion::identity();
		for (int k = 0; k < 18; k++) q = q * rot;
		assert(areEquivalent(q, Quaternion::identity()));
		//assert(areEqual(q, Quaternion::identity())); //This doesn't work, but it's ok because q and -q are the same rotation
	}
}

void unitTestTransformation()
{
	//Testing inverse
	Transform t;
	t.rotate = Quaternion::fromAngleAxis(43.4f, Vector3(1, -3, -2));
	t.translate = Vector3(1, 3, 4);
	t.scale = 5.f;
	Point3 p(4, 10, -13);
	Point3 q = t.transformPoint(p);
	Point3 r = t.inverse().transformPoint(q);

	assert(areEqual(p, r));

	//Testing invert
	Transform ti = t;
	ti.invert();
	r = ti.transformPoint(q);

	assert(areEqual(p, r));
}

//ASCII art: convert an intensity value [0,1] into a sequence of two chars
const char* intensityToCstr(Scalar intensity)
{
	switch(int (std::round(intensity*5)))
	{
		case 0: return "  "; //darkest
		case 1: return " \'";
		case 2: return " +";
		case 3: return " *";
		case 4: return " #";
		default: case 5: return "##"; //lightest
	}
}

float currentTime()
{
	static float now = .0f;
	now += .005f;
	return now;
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
	float time = currentTime();
	Camera c(2.f, 30, 30);
	Sphere sphere(Point3(0, 0, 6), 2);
	Sphere sphere1(Point3(2.f * std::cos(time), 1, 6 + 2.f * std::sin(time)), 1);
	Plane plane(Point3(0, -1.5f, 0), Versor3(0, 1, 0));

	std::string screenBuffer; //a string to get ready and print all at once

	for(int y = 0; y < c.pixelDimY; y++)
	{
		for(int x = 0; x < c.pixelDimX; x++)
		{
			Point3 hitPos;
			Versor3 hitNorm;
			Scalar distMax = 100000.f;
			rayCast(c.primaryRay(x, y), sphere, hitPos, hitNorm, distMax);
			rayCast(c.primaryRay(x, y), sphere1, hitPos, hitNorm, distMax);
			rayCast(c.primaryRay(x, y), plane, hitPos, hitNorm, distMax);

			screenBuffer += lighting(hitNorm);
			//std::cout << lighting(hitNorm);
		}
		//std::cout << std::endl;
		screenBuffer += '\n';
	}
	std::cout << screenBuffer;
}

int main()
{
	unitTestProducts();
	unitTestLinearOps();
	unitTestRaycasts();
	unitTestRaycastPlane();
	unitTestQuaternions();
	unitTestTransformation();
	//while(1)
		//rayCastingSphere();
}