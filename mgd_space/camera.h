#pragma once

#include "vector3.h"
#include "shapes3d.h"

namespace mgd
{
	class Camera
	{

	public:

		Scalar focal; //Distance of the image plane
		int pixelDimX, pixelDimY;

		Camera(Scalar _f, int sx, int sy) : focal(_f), pixelDimX(sx), pixelDimY(sy) {}

		Ray primaryRay(int pixelX, int pixelY)
		{
			Ray r;
			r.p = Point3(0, 0, 0); //The point of view is on the origin by definition
			Scalar clipX = 2.f * pixelX / pixelDimX - 1.f; //the *2-1 is to make sure that the value is in [-1, 1] and not [0, 1]
			Scalar clipY = -2.f * pixelY / pixelDimY + 1.f;
			r.d = Vector3(clipX, clipY, focal).normalized();
			return r;
		}
	};
}
