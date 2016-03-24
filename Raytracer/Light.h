#pragma once
#include "Material.h"
#include "GenVector.h"
#include "objLoader.h"

class Light
{
public:
	Light(obj_vector *points, Material *mat,float intensity);
	~Light();
	Vector3 Ka, Kd, Ks,origin;
	float intensity;
private:

};

Light::Light(obj_vector *points, Material *mat,float intensity): Ka(mat->Ka), Kd(mat->Kd), Ks(mat->Ks),origin(objToGenVec(points)),intensity(intensity)
{
}

Light::~Light()
{
}