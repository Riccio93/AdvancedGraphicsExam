#include <iostream>
#include <conio.h>
#include <windows.h>
#include "vector3.h"
#include "shapes3d.h"
#include "camera.h"
#include "quaternion.h"
#include "transform.h"
#include "scene.h"

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

	//Testing composition
	Transform tA;
	tA.rotate = Quaternion::fromAngleAxis(-13.f, Vector3(13, 4, 0));
	tA.translate = Vector3(0, -1, 1);
	tA.scale = .23f;
	Transform tB = t;

	assert(areEqual((tA * tB).transformPoint(p), tA.transformPoint(tB.transformPoint(p))));
}

void runUnitTests()
{
	unitTestProducts();
	unitTestLinearOps();
	unitTestRaycasts();
	unitTestRaycastPlane();
	unitTestQuaternions();
	unitTestTransformation();
}

float currentTime()
{
	static float now = .0f;
	now += .005f;
	return now;
}

void printInstructions(bool isFirstPerson, GameObj* currentGameObj)
{
	std::cout << "CURRENT MODE: " << (isFirstPerson ? "FIRST PERSON" : "THIRD PERSON") << std::endl
		<< "BUTTONS: W FORWARD, S BACKWARDS, A TURN ANTI-CLOCKWISE, D TURN CLOCKWISE\n"
		<< "CURRENT COORDINATES: " 
		<< currentGameObj->transform.translate.x << ' '
		<< currentGameObj->transform.translate.y << ' '
		<< currentGameObj->transform.translate.z << std::endl;
	std::cout << currentGameObj;
}

void printOnScreen(Scene s, GameObj* currentGameObj, bool isFirstPerson)
{
	isFirstPerson ? rayCasting(s.toView(currentGameObj->getTransform())) : rayCasting(s.toWorld());
	printInstructions(isFirstPerson, currentGameObj);
}

void inputHandling(const Scene &s, GameObj*& currentGameObj, const std::vector<GameObj*> &objVec, Scalar stepLength, Scalar rotationAngle, bool &isFirstPerson)
{
	Vector3 translationToApply;
	Quaternion rotationToApply = Quaternion::identity();

	char input = _getch(); //Gets the character pressed without the need to press ENTER

	if(input >= '0' && input <= '9')
	{
		input -= '0';
		currentGameObj = (objVec[input]);
	}
	else
	{
		switch (input)
		{
		case 'w':
			translationToApply = currentGameObj->transform.forwardDir() * stepLength;
			break;
		case 's':
			translationToApply = -currentGameObj->transform.forwardDir() * stepLength;
			break;
		case 'a':
			rotationToApply = Quaternion::fromAngleAxis(-rotationAngle, Vector3::up());
			break;
		case 'd':
			rotationToApply = Quaternion::fromAngleAxis(rotationAngle, Vector3::up());
			break;
		case 32: //SPACE
			isFirstPerson = !isFirstPerson;
			break;
		case 'q':
			exit(0); //Closes the console
			break;
		}
		currentGameObj->move(translationToApply);
		currentGameObj->rotate(rotationToApply);
	}	
	printOnScreen(s, currentGameObj, isFirstPerson);
}

int main()
{
	bool isFirstPerson = false;
	Scalar stepLength = 2.5f;
	Scalar rotationAngle = 15.f;

	std::srand((unsigned int)time(NULL)); //Initializes the random seed

	HWND console = GetConsoleWindow();
	ShowWindow(console, SW_SHOWMAXIMIZED); //Makes sure the console window opens maximized

	Scene s;
	std::vector<GameObj*> objVec = s.populate(10);
	GameObj* currentCharacter = objVec[0];

	printOnScreen(s, currentCharacter, isFirstPerson);

	while(1)
	{
		inputHandling(s, currentCharacter, objVec, stepLength, rotationAngle,isFirstPerson);
	}	
}