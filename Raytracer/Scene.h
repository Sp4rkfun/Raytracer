#pragma once
#include <vector>
#include <memory>
#include "objLoader.h"

#include "Light.h"
#include "Material.h"
#include "Camera.h"

#include "RenderPrimitive.h"

using namespace std;

typedef shared_ptr<RenderPrimitive> Shape;

class Scene
{
public:
	Scene(objLoader *loader);
	~Scene();
	objLoader *loader;
	vector<Material*> materials;
	vector<Light*> lights;
	vector<RenderPrimitive*> objects;
	Camera camera;
	void reloadMaterials() {
		int sz = materials.size();
		for (int i = 0;i < sz;++i) {
			delete materials[i];
		}
		sz = loader->materialCount;
		materials.resize(sz);
		for (int i = 0;i < sz;++i) {
			materials[i] = new Material(loader->materialList[i]);
		}
	}
	void reloadLights() {
		int sz = lights.size();
		for (int i = 0;i < sz;++i) {
			delete lights[i];
		}
		sz = loader->lightPointCount;
		if (sz > 0) {
			float intensity = 1.0 / sz;
			lights.resize(sz);
			for (int i = 0;i < sz;++i) {
				obj_light_point *o = loader->lightPointList[i];
				lights[i] = new Light(loader->vertexList[o->pos_index], materials[o->material_index], intensity);
			}
		}
	}

	void reloadObjects() {
		int sz = objects.size();
		for (int i = 0; i < sz; ++i)
		{
			delete objects[i];
		}
		sz = loader->sphereCount;
		objects.resize(sz+loader->faceCount);

		for (int i = 0; i<sz; ++i)
		{
			obj_sphere *o = loader->sphereList[i];
			Vector3 origin = objToGenVec(vertex(o->pos_index));
			float radius = loader->normalList[o->up_normal_index]->e[0];
			//,materials[o->material_index]
			Sphere *s = new Sphere(origin, radius, materials[o->material_index]);
			objects[i]=s;
		}
		sz += loader->faceCount;
		for (int i = loader->sphereCount; i<sz; ++i)
		{
			obj_face *o = loader->faceList[i];

			Triangle *t = new Triangle(objToGenVec(loader->vertexList[o->vertex_index[0]]),
				objToGenVec(loader->vertexList[o->vertex_index[1]]),
				objToGenVec(loader->vertexList[o->vertex_index[2]]),
				materials[o->material_index]
				);
			objects[i]=t;
		}
	}
	void initialize() {
		reloadMaterials();
		reloadLights();
		reloadObjects();
	}

private:
	inline obj_vector *vertex(int index) {
		return loader->vertexList[index];
	}
};

Scene::Scene(objLoader *loader) :loader(loader), camera(
	objToGenVec(vertex(loader->camera->camera_look_point_index)),
	objToGenVec(loader->normalList[loader->camera->camera_up_norm_index]).normalize(),
	objToGenVec(vertex(loader->camera->camera_pos_index))
	)
{
}

Scene::~Scene()
{
}