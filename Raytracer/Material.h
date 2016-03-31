#pragma once
#include "GenVector.h"
#include "objLoader.h"

class Material
{
public:
	Material(obj_material *mtl);
	~Material();
	Vector3 Ka, Kd, Ks;
	float reflect,shiny;
private:

};

Material::Material(obj_material *mtl):
	Ka(objToGenVec(mtl->amb)),Kd(objToGenVec(mtl->diff)),Ks(objToGenVec(mtl->spec)),
	reflect(mtl->reflect),shiny(mtl->shiny)
{
}

Material::~Material()
{
}