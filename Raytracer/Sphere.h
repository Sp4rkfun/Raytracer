#pragma once
#include "RenderPrimitive.h"

class Sphere : public RenderPrimitive
{
public:
	Sphere(Vector3 &origin, float radius, Material *mat);
	~Sphere();
	float intersects(Ray &r) override;
	Vector3 origin;
	Vector3 getNormal(Vector3 &hit) override;
private:
	float rSquared;
};

Sphere::Sphere(Vector3 &origin, float radius,Material *mat): RenderPrimitive(mat), origin(origin),rSquared(pow(radius,2))
{
	Vector3 rad = Vector3(radius, radius, radius);
	maximum = origin + rad;
	minimum = origin - rad;
	//printf("%f %f %f", origin[0], origin[1], origin[2]);
}

Sphere::~Sphere()
{
}
Vector3 Sphere::getNormal(Vector3 &hit) {
	//printf("%f\n",(hit - origin).normalize()[0]);
	return (hit - origin).normalize();
}

float Sphere::intersects(Ray &r) {
	Vector3 ec = (r.origin-origin);
	float a = r.dir.dot(r.dir);
	float b =  (2.0f*r.dir).dot(ec);
	float c = ec.dot(ec) - rSquared;
	float disc = pow(b, 2) - 4*a*c;
	if (disc < 0)return -1;
	else if (disc>0) {
		//printf("win");
		disc = sqrt(disc);
		float sum = -b - disc;
		if (sum >= 0)return sum / (2*a);
		else return (-b + disc) /(2*a);
	}
	else return -b / (2*a);//return sqrt(ec.dot(ec)-pow(ec.dot(r.dir),2));
	//if(discriminant>0)
	//printf("%f\n", discriminant);
}