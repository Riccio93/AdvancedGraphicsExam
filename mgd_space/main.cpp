#include "vector3.h"
#include "shapes3d.h"

using namespace mgd;

void UnitTestLinearOps()
{
	Vector3 a(24, 1, 32), b(12, 3, 54);
	Scalar k = 5.f;

	assert(areEqual(a + b, b + a));	
	assert(areEqual((a + b) * k, b * k + a * k));
}

void UnitTestProducts()
{
	Vector3 a(24, -3, 32), b(12, 3, -54), c(10, 13, -43);
	Scalar k = .4f;

	assert(isZero(dot(cross(a, b), a)));
	assert(isZero(dot(cross(a, b), b)));
	assert(areEqual(dot(k*a, b), dot(b,k*a)));
	assert(areEqual(cross(a, b + c), cross(a,b) + cross(a, c)));
}

void UnitTestRaycasts()
{
	Ray r(Point3(0, 0, 0), Vector3(1, 0, 0));
	Sphere s(Point3(5, 0, 0), 3);
}

int main()
{
	UnitTestLinearOps();
	UnitTestProducts();

	Vector3 v(0, 2, 3);
	Vector3 r = v * 4;
	r *= 0.25f;
	Vector3 w = v + r;
	v += w;

	v = -w;

	Scalar k = dot(v, w);

	Scalar q = v[0];
}