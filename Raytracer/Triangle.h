#pragma once
#include "RenderPrimitive.h"

class Triangle : public RenderPrimitive
{
public:
	Triangle(Vector3 &v1, Vector3 &v2, Vector3 &v3, Material *mat);
	~Triangle();
	float intersects(Ray &r) override;
	Vector3 getNormal(Vector3 &hit) override;
	Vector3 p1,p2,p3,e1,e2,e3,normal;
private:

};

Triangle::Triangle(Vector3 &v1, Vector3 &v2, Vector3 &v3,Material *mat) :RenderPrimitive(mat), p1(v1), p2(v2), p3(v3),
e1(v1 - v2), e2(v2 - v3), e3(v1 - v3),
normal((e1).cross(e2).normalize())//(v3-v1
{
	minimum = v1;
	maximum = v1;
	for (size_t i = 0; i < 3; ++i)
	{
		float val = v2[i];
		if (val < minimum[i]) minimum[i] = val;
		else if (val > maximum[i]) maximum[i] = val;
	}
	for (size_t i = 0; i < 3; ++i)
	{
		float val = v3[i];
		if (val < minimum[i]) minimum[i] = val;
		else if (val > maximum[i]) maximum[i] = val;
	}
}

Triangle::~Triangle()
{
}

Vector3 Triangle::getNormal(Vector3 &hit) {
	return normal;
}

float Triangle::intersects(Ray &r) {
	//if((v1))
	float nDotdir = normal.dot(r.dir);
	//if (nDotdir < 0.001)return -1;
	float t = ((p1 - r.origin).dot(normal)) / nDotdir;
		//(normal.dot(r.origin) + D) / nDotdir;
	if (t < 0)return -1;
	//printf("%f\n", t);
	Vector3 hit(r.origin + t*r.dir);
	Vector3 h1((p2-p1).cross(hit - p1));
	if (normal.dot(h1) < 0)return-1;

	Vector3 h2((p3-p2).cross(hit - p2));
	if (normal.dot(h2) < 0)return-1;

	Vector3 h3((p1-p3).cross(hit - p3));
	if (normal.dot(h3) < 0)return-1;
	//printf("asdf");
	return t;
}