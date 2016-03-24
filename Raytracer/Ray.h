#pragma once
#include "GenVector.h"
class Ray
{
public:
	Ray(Vector3 origin, Vector3 dir);
	~Ray();
	Vector3 origin;
	Vector3 dir;
	inline Vector3 &getDirection() {
		return dir;
	}
private:

};

Ray::Ray(Vector3 origin, Vector3 dir):origin(origin),dir(dir)
{
	
}

Ray::~Ray()
{
}