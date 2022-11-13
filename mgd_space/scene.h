#pragma once

#include <iostream>
#include <vector>
#include "transform.h"
#include "shapes3d.h"
#include "camera.h"
#include "gameObj.h"

namespace mgd
{
	class Scene
	{

	public:

		std::vector<GameObj*> obj; //A set of gameobj (each with its own transform)

		std::vector<GameObj*> populate(int n)
		{
			for(int i=0; i<n; i++)
			{
				GameObj* newObj = new GameObj();
				if(i==0)
					newObj->transform.translate = Vector3(0, 0, 10);
				else
				{
					newObj->transform.translate = Vector3::random(25); //The y is divided by 10 inside the method
					newObj->scale(.75f + (std::rand() % 1000) / Scalar(1000) * (7.f - .5f)); //Scale is in interval [0.75, 7]
				}
				obj.push_back(newObj);
			}
			return obj;
		}

		//Local to world coordinates
		std::vector<Sphere> toWorld() const
		{
			std::vector<Sphere> res;
			std::vector<GameObj*>::const_iterator citer;
			for (citer = obj.cbegin(); citer != obj.cend(); citer++)
			{
				res.push_back(apply((*citer)->getTransform(), (*citer)->body));
			}
			return res;
		}

		//Local to view coordinates
		std::vector<Sphere> toView(Transform camera) const
		{
			std::vector<Sphere> res;
			std::vector<GameObj*>::const_iterator citer;
			for (citer = obj.cbegin(); citer != obj.cend(); citer++)
			{
				Sphere current = apply((*citer)->getTransform(), (*citer)->body);
				res.push_back(apply(camera.inverse(), current));
			}
			return res;
		}
	};

	//ASCII art: convert an intensity value [0,1] into a sequence of two chars
	const char* intensityToCstr(Scalar intensity)
	{
		switch (int(std::round(intensity * 8)))
		{
		case 0: return "  "; //darkest
		case 1: return " \'";
		case 2: return " +";
		case 3: return " *";
		case 4: return " #";
		case 5: return "\'#";
		case 6: return "+#";
		case 7: return "*#";
		case 8: default: return "##"; //lightest
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

	void rayCasting(const std::vector<Sphere> &sphereVector)
	{
		Camera c(2.f, 50, 50);
		Plane plane(Point3(0, -1.5f, 0), Versor3(0, 1, 0));

		std::string screenBuffer; //a string to get ready and print all at once

		for (int y = 0; y < c.pixelDimY; y++)
		{
			for (int x = 0; x < c.pixelDimX; x++)
			{
				Point3 hitPos;
				Versor3 hitNorm;
				Scalar distMax = 1000.f;

				rayCast(c.primaryRay(x, y), plane, hitPos, hitNorm, distMax); //Floor raycast
				for(const Sphere &s : sphereVector)
				{
					rayCast(c.primaryRay(x, y), s, hitPos, hitNorm, distMax); //Spheres raycast
				}
				screenBuffer += lighting(hitNorm);
			}			
			screenBuffer += '\n';
		}
		std::cout << screenBuffer;
	}
} //End of namespace mgd