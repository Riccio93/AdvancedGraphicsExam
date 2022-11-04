#include "vector3.h"

using namespace mgd;

void UnitTestLinearOps()
{

}

int main()
{
	Vector3 v(0, 2, 3);
	Vector3 r = v * 4;
	r *= 0.25f;
	Vector3 w = v + r;
	v += w;

	v = -w;

	Scalar k = dot(v, w);

	Scalar k = v[0];
}