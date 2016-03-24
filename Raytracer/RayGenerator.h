#pragma once
#include "Camera.h"
#define _USE_MATH_DEFINES
#include "Math.h"
#include "Ray.h"
class RayGenerator
{
public:
	RayGenerator(Camera &c,int32_t xRes,int32_t yRes);
	~RayGenerator();
	Ray getRay(int32_t x, int32_t y);
	float d;
	Camera camera;
private:
	float l, r, t, b;
	uint32_t xRes, yRes;
};

RayGenerator::RayGenerator(Camera &c, int32_t xRes, int32_t yRes):camera(c),l(-xRes/2),
r(xRes/2),t(yRes/2),b(-yRes/2),xRes(xRes),yRes(yRes)
{
	d =  ((float) t)*sin(M_PI_2);

}

RayGenerator::~RayGenerator()
{
}

Ray RayGenerator::getRay(int32_t x, int32_t y) {
	float u = l + (r - l)*(x + 0.5) / xRes;
	float v = b + (t - b)*(y + 0.5) / yRes;
	Vector3 s(-(d*camera.w) + u*camera.u + v*camera.v);
	return Ray(camera.e, s.normalize());
}