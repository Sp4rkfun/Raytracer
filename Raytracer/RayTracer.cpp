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
	Ray r(point.pt, (light.origin - point.pt).normalize());
	int sz = points.size();
	for (size_t i = 0; i < sz; ++i)
	{
		if (points[i]->intersects(r)>-1)return false;
	}
	return true;
}
Vector3 shadeLights(Hitpoint &hitpoint,vector<RenderPrimitive*> &points,vector<Light*> &lights) {
	Vector3 vec;
	int sz = lights.size();
	for (size_t j = 0; j < sz; ++j)
	{
		auto &light = *lights[j];
		auto &mat = *hitpoint.mat;
		vec += light.Ka*mat.Ka;
		if (isClosest(hitpoint, points,light)) {

			auto &v = hitpoint.v;
			auto &n = hitpoint.n;
			auto l = (light.origin - hitpoint.pt).normalize();
			auto I = 1/(light.origin).distanceSquared(hitpoint.pt);//light.intensity;
			auto h = (v + l).normalize();

			float distsq = (light.origin).distanceSquared(hitpoint.pt);

			vec+= mat.Kd*light.Kd*max(0, n.dot(l)) + mat.Ks*light.Ks*pow(max(0, n.dot(h)), mat.shiny);
			
			//v += mat.Kd*I*max(0, n.dot(l));
		}
	}
	//if (vec[0] > 0)printf("%f, %f, %f\n",vec[0],vec[1],vec[2]);
	return vec;
}

Vector3 shadows(Hitpoint &hitpoint, vector<RenderPrimitive*> &points, vector<Light*> &lights) {
	auto &mat = *hitpoint.mat;
	Vector3 vec;
	int sz = lights.size();
	for (size_t j = 0; j < sz; ++j)
	{
		auto &light = *lights[j];
	}
	return vec;
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
	Buffer<Color> buffer(RES, RES);

	//load camera data
	objLoader objData = objLoader();
	objData.load(argv[1]);
	Scene s(&objData);
	s.initialize();
	auto &camera = s.camera;
	auto &objects = s.objects;
	auto &lights = s.lights;
	RayGenerator generator = RayGenerator(camera, RES, RES);

	//Convert vectors to RGB colors for testing results
	clock_t start = clock();
	parallel_for(size_t(0), size_t(RES), [&](size_t y)
		//for (int y = 0; y < RES;++y)
	{
		for (int x = 0; x < RES; ++x)
		{
			float closest = FLT_MAX;
			int index = -1;
			Ray r = generator.getRay(x, y);
			//printf("%f\n",r.dir[2]);
			Vector3 rt = r.getDirection();
			bool intersected = false;
			for (size_t i = 0; i < objects.size(); ++i)
			{
				float t = objects[i]->intersects(r);
				if (t > -1) {
					if (t < closest) {
						index = i;
						closest = t;
					}
					//r.dir[0] = 1; r.dir[1] = 1; r.dir[2] = 1;
				}
			}
			if (index > -1) {
			//	rt = objects[index]->getNormal(r.origin + r.dir*closest);
				Vector3 point = r.origin + closest*r.dir;
				Material *mat = objects[index]->getMaterial();
				Hitpoint hit(point, -r.dir, objects[index]->getNormal(point),mat);
				Vector3 d = shadeLights(hit,objects,lights)*255.0;
				Color c = Color(fabs(d[0]), fabs(d[1]), fabs(d[2]));
				buffer.at(x, RES - y - 1) = c;
				//printf("%f\n", d[2]);
			}
			else {
				Vector3 d = rt*255.0f;
				Color c = Color(fabs(d[0]), fabs(d[1]), fabs(d[2]));
				buffer.at(x, RES - y - 1) = c;
			}
		}
	});
	printf("Rendered in %d ms\n", (clock() - start));
	//Write output buffer to file argv2
	simplePNG_write(argv[2], buffer.getWidth(), buffer.getHeight(), (unsigned char*)&buffer.at(0, 0));

	return 0;
}
