#define _CRT_SECURE_NO_DEPRECATE
#include "GenVector.h"
#include "simplePNG.h"
#include "Buffer.h"
#include "objLoader.h"
#include "RayGenerator.h"
#include "Material.h"
// For the test scenes, resolution of 100x100, and fov 90 degree, my
// generator creates the test images. My ray dirs are normalized.
#include "Sphere.h"
#include "Triangle.h"
#include "Light.h"
//Hard code resolution for now
#define RES 100

#define _USE_MATH_DEFINES //This enables math constants in Windows
#include <math.h> //Math functions and some constants

#include "Ray.h"
#include "Camera.h"
#include <vector>
#include <memory>
#include "ppl.h"
#include "Scene.h"
#include "windows.h"
#include "GenVector.h"
#include "Hitpoint.h"
#include "BoundingBox.h"

using namespace std;
using namespace concurrency;

//This might be helpful to convert from obj vectors to GenVectors

void printVector(obj_vector *v)
{
	printf("%.2f,", v->e[0]);
	printf("%.2f,", v->e[1]);
	printf("%.2f  ", v->e[2]);
}

bool isClosest(Hitpoint &point,vector<RenderPrimitive*> &points,Light &light) {
	float dist = point.pt.distance(light.origin);
	Ray r(point.pt, (light.origin - point.pt).normalize());
	int sz = points.size();
	for (size_t i = 0; i < sz; ++i)
	{
		float between = points[i]->intersects(r);
		if (between>-1&&dist>between)return false;
	}
	return true;
}

Vector3 shadeLights(Hitpoint &hitpoint,BoundingBox &box,vector<Light*> &lights) {
	Vector3 vec;
	int sz = lights.size();
	for (size_t j = 0; j < sz; ++j)
	{
		auto &light = *lights[j];
		auto &mat = *hitpoint.mat;
		vec += light.Ka*mat.Ka;
		Ray r(hitpoint.pt, (light.origin - hitpoint.pt).normalize());
		RenderPrimitive *hit = box.getIntersect(r);
		if (hit != nullptr) {
			float dist = hitpoint.pt.distance(light.origin);
			float between = hit->intersects(r);
			if (dist > between&&between > -1)continue;
		}
			auto &v = hitpoint.v;
			auto &n = hitpoint.n;
			auto l = (light.origin - hitpoint.pt).normalize();
			auto I = 1/(light.origin).distanceSquared(hitpoint.pt);//light.intensity;
			auto h = (v + l).normalize();

			float distsq = (light.origin).distanceSquared(hitpoint.pt);
			vec += mat.Kd*light.Kd*max(0, n.dot(l)) + mat.Ks*light.Ks*pow(max(0, n.dot(h)), mat.shiny);vec+= mat.Kd*light.Kd*max(0, n.dot(l)) + mat.Ks*light.Ks*pow(max(0, n.dot(h)), mat.shiny);		
	}
	return vec;
}

Vector3 mirror(Ray &origin, BoundingBox &box, vector<Light*> lights,int reflections) {

	float closest = FLT_MAX;
	int index = -1;
	RenderPrimitive *r =box.getIntersect(origin);
	if (r == nullptr) return Vector3();
	printf("found!");
	float dist = r->intersects(origin);
	/*int sz = points.size();
	for (size_t i = 0; i < sz; ++i)
	{
		float dist = points[i]->intersects(origin);
		if (dist>-1 && dist<closest) {
			closest = dist;
			index = i;
		}
	}*/
	if (dist == -1) return Vector3();
	else 
	{
		Vector3 pt = origin.origin + origin.dir*closest;
		Hitpoint hp(pt, -origin.dir, r->getNormal(pt), r->material);
		Vector3 c = shadeLights(hp, box, lights);
		if (reflections == 0)
			return c;//points[index]->material->Ks;
		else {

			return c + r->material->reflect*mirror(Ray(hp.pt, origin.dir.reflect(hp.n)),box,lights,--reflections);
		}
	}
}

int main(int argc, char ** argv)
{
	//TODO: create a frame buffer for RESxRES


	//Need at least two arguments (obj input and png output)
	if (argc < 3)
	{
		printf("Usage %s input.obj output.png\n", argv[0]);
		exit(0);
	}
	Buffer<Vector3> buffer(RES, RES);

	//load camera data
	objLoader objData = objLoader();
	objData.load(argv[1]);
	Scene s(&objData);
	s.initialize();
	BoundingBox *box = new BoundingBox(s.objects);
	auto &camera = s.camera;
	auto &objects = s.objects;
	auto &lights = s.lights;
	RayGenerator generator = RayGenerator(camera, RES, RES);

	//Convert vectors to RGB colors for testing results
	clock_t start = clock();
	parallel_for(size_t(0), size_t(RES), [&](size_t y)
	{
		for (int x = 0; x < RES; ++x)
		{
			Ray r = generator.getRay(x, y);
				Vector3 d = mirror(r,*box,lights,1);
				Color c = Color(fabs(d[0]), fabs(d[1]), fabs(d[2]));
				buffer.at(x, RES - y - 1) = d;
		}
	});
	float scaling = 0;
	for (size_t y = 0; y < RES; ++y)
	{
		for (int x = 0; x < RES; ++x)
		{
			scaling = max(scaling, buffer.at(x, y).maxComponentValue());
		}
	}
	scaling /= 255.0;
	printf("Scaling %f\n", scaling);
	Buffer<Color> colors(RES, RES);
	parallel_for(size_t(0), size_t(RES), [&](size_t y)
	{
		for (int x = 0; x < RES; ++x)
		{
			Vector3 &d = buffer.at(x, y)/scaling;
			Color c = Color(fabs(d[0]), fabs(d[1]), fabs(d[2]));
			colors.at(x, y)=c;
		}
	});
	printf("Rendered in %d ms\n", (clock() - start));
	//Write output buffer to file argv2
	simplePNG_write(argv[2], colors.getWidth(), colors.getHeight(), (unsigned char*)&colors.at(0, 0));

	return 0;
}
