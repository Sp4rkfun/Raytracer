#pragma once
#include "GenVector.h"
#include "Ray.h"
class RenderPrimitive
{
public:
	Material *material;
	RenderPrimitive(Material *mat);
	virtual ~RenderPrimitive();
	virtual float intersects(Ray &r) = 0;
	virtual Vector3 getNormal(Vector3 &hit) = 0;
	Vector3 objToGenVec(obj_vector const * objVec)
	{
		Vector3 v;
		v[0] = (float) objVec->e[0];
		v[1] = (float) objVec->e[1];
		v[2] = (float) objVec->e[2];
		return v;
	}
	Material *getMaterial() {
		return material;
	}
private:

};

RenderPrimitive::RenderPrimitive(Material *mat):material(mat)
{
}

RenderPrimitive::~RenderPrimitive()
{
}