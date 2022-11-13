#pragma once

namespace mgd
{
	class GameObj
	{

	public:

		Transform transform;

		//Insert here content's description (mesh, collider, etc. (Everything in local space!))
		//For now all objects are spheres with "noses"

		Sphere body;
		GameObj() :
			transform(Transform()),
			body(Vector3(0, 0, 0), 1.f) { }

		void move(const Vector3& offset) { transform.translate += offset; }
		void rotate(const Quaternion& offset) { transform.rotate = offset * transform.rotate; }
		void scale(Scalar value) { transform.scale *= value; }

		Transform getTransform() const { return transform; }
		Sphere getBody() const { return body; }
	};

} //end of namespace mgd