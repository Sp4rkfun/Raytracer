#pragma once
#include "GenVector.h"
#include "Material.h"
const float epsilon = 0.00011;
class Hitpoint
{
public:
	Vector3 v;
	Vector3 n;
	Vector3 pt;
	Material *mat;
	Hitpoint(Vector3 pt,Vector3 v, Vector3 n, Material *mat);
	~Hitpoint();

private:

};

Hitpoint::Hitpoint(Vector3 pt,Vector3 v,Vector3 n,Material *mat):v(v),n(n),pt(pt+n*epsilon),mat(mat)
{
}

Hitpoint::~Hitpoint()
{
}