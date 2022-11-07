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
				//newObj.transform.translate = Vector3::random(10) + Vector3(0,0,10);
				newObj->transform.translate = Vector3(0, 0, 5);
				//newObj->transform.translate.y = 0;
				obj.push_back(newObj);
			}
			return obj;
		}

		//Makes a vector of spheres in world space
		std::vector<GameObj*> toWorld() const
		{
			std::vector<GameObj*> res;
			for(GameObj* g : obj)
			{
				//res.push_back(apply(g.transform, g.nose));
				res.push_back(apply(g->transform, g->body));
			}
			return res;
		}

		std::vector<Sphere> toView(Transform camera) const
		{
			//Trasforma ogni sfera con la sua trasformazione cumulata con l'inversa della camera, per
			//portarlo in spazio vista.
			//Ripetere il rendering ad ogni key press (con una readchar), con wasd controllo la camera (a e d la
			//sfera ruota).
			//Quale sfera ruota? Ci sarà una variablie currentPlayer, che è un elemento dell'array e ogni
			//volta che mi muovo cambio la trasformazione di currentplayer.
			//Con il tasto spazio passo dalla third person mode (fuori da tutto) alla first person mode,
			//assegnando la sua trasformazione al method toview (ha come camera la trasformazione associata al'oggetto
			//i-esimo.
			//Coi tasti da 0 a 9 si cambia personaggio

			/*std::vector<Sphere> res;
			for(const GameObj &g : obj)
			{
				res.push_back(apply(camera.inverse(), apply(g.transform, g.body)));
			}
			return res;*/
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
		Camera c(2.f, 60, 60);
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