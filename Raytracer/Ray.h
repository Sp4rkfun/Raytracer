#pragma once
#include "GenVector.h"
class Ray
{
public:
	Ray(Vector3 origin, Vector3 dir);
	~Ray();
	Vector3 origin;
	Vector3 dir;
	inline Vector3 &getDirection() const{
		return (Vector3& const) dir;
	}
	inline Vector3 &getOrigin() const {
		return (Vector3& const)dir;
	}

private:

};

Ray::Ray(Vector3 origin, Vector3 dir):origin(origin),dir(dir)
{
	
}

Ray::~Ray()
{
}