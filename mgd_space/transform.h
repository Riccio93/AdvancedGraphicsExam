#pragma once

#include "quaternion.h"

namespace mgd
{
	class Transform
	{

	public:

		Transform() : scale(1), translate(0, 0, 0), rotate(Quaternion::identity()) { }

		Scalar scale;
		Vector3 translate;
		Quaternion rotate;

		Point3 transformPoint(const Point3 &p) const
		{
			return rotate.apply(p * scale) + translate;
		}

		Vector3 transformVector(const Vector3 &p) const
		{
			return rotate.apply(p * scale);
		}

		Versor3 transformVersor(const Versor3 &p) const
		{
			return rotate.apply(p);
		}

		Scalar transformScalar(Scalar p) const { return p * scale; }

		void invert()
		{
			scale = 1 / scale;
			rotate.conjugate();
			translate = rotate.apply(-translate * scale); //alternatively: translate = applyToVector3(-translate)			
		}

		Transform inverse() const
		{
			Transform t;
			t.scale = 1 / scale;
			t.rotate = rotate.conjugated();
			t.translate = t.rotate.apply(-translate * t.scale); //alternatively: t.translate = t.applyToVector3(-translate)
			return t;
		}

		Vector3 forwardDir() const
		{
			return rotate.apply(Vector3(0, 0, 1));
		}

	};

	//b first, then a
	Transform operator * (const Transform &a, const Transform &b)
	{
		Transform t;
		t.rotate = a.rotate * b.rotate;
		t.scale = a.scale * b.scale;
		t.translate = a.transformVector(b.translate) + a.translate;
		return t;
	}


} //end of namespace mgd

