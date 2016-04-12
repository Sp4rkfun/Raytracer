#pragma once
#include <vector>

#include "GenVector.h"
#include "RenderPrimitive.h"
#include "Hitpoint.h"
class  BoundingBox
{
public:
	RenderPrimitive *leaf=nullptr;
	BoundingBox *left=nullptr, *right=nullptr;
	Vector3 minimum, maximum;
	BoundingBox(std::vector<RenderPrimitive*> points);
	 BoundingBox(Vector3 minimum, Vector3 maximum, std::vector<RenderPrimitive*> points);
	~BoundingBox();
	virtual float intersect(Ray const & ray) const
	{
		//we want to find the farthest entrace and closest exit to the box
		//if the exit is closer than the entrance, there is no hit
		const size_t vecDim = 3;
		float entrance = 0.0f;
		//float exit = ;
		const Vector3 &dir = ray.dir;
		const Vector3 &orig = ray.origin;
		Vector3 invDir(1.0f / dir[0], 1.0f / dir[1], 1.0f / dir[2]);
		float t1 = (minimum[0] - orig[0])*invDir[0];
		float t2 = (maximum[0] - orig[0])*invDir[0];
		float t3 = (minimum[1] - orig[1])*invDir[1];
		float t4 = (maximum[1] - orig[1])*invDir[1];
		float t5 = (minimum[2] - orig[2])*invDir[2];
		float t6 = (maximum[2] - orig[2])*invDir[2];

		float tmax = max(max(min(t1, t2), min(t3, t4)), min(t5, t6));
		float tmin = min(min(max(t1, t2), max(t3, t4)), max(t5, t6));
		//Box behind
		if (tmax < 0)return -1;
		//printf("%f :: %f\n",tmin,tmax);
		//No intersection
		if (tmin > tmax)return -1;
		printf("hit box\n");
		return tmin;
	}
	RenderPrimitive *getIntersect(Ray const & ray) {
		return recurIntersect(ray,intersect(ray));
	}
	RenderPrimitive *recurIntersect(Ray const &ray,float dist) {
		if (dist > -1) {
			printf("recursing\n");
			if (leaf != nullptr)return leaf;
			float ld = left->intersect(ray);
			float rd = right->intersect(ray);
			if (ld == -1)return right->recurIntersect(ray, rd);
			else if (rd == -1 || ld<rd)return left->recurIntersect(ray, ld);
			else return right->recurIntersect(ray, rd);
		}
		return nullptr;
	}
private:

};
BoundingBox::BoundingBox(std::vector<RenderPrimitive*> points) {
	int sz = points.size();
	for (size_t i = 1; i < sz; ++i)
	{
		RenderPrimitive *x = points[i];
		int j = i-1;
		printf("%d :: %d\n", j + 1, sz);
		while (j >= 0 && points[j]->maximum[0]>x->maximum[0])
		{
			printf("\t%d :: %d", j + 1, sz);
			points[j + 1] = points[j];
			--j;
		}
		points[j + 1] = x;
	}
}
BoundingBox::BoundingBox(Vector3 minimum, Vector3 maximum, std::vector<RenderPrimitive*> points)
{
	int sz = points.size();
	if (sz == 1) { leaf = points[0]; this->minimum = minimum; this->maximum = maximum; }
	else {
		Vector3 difference = maximum - minimum;
		int axis = difference.maxComponent();
		std::vector<RenderPrimitive*> lbox, rbox;
		Vector3 lmin, lmax, rmin, rmax;

		float mid = minimum[axis] + (difference[axis] / 2.0);
		for (size_t i = 0; i < sz; i++)
		{
			RenderPrimitive *point = points[i];
			if (point->minimum[axis] < mid) {
				lbox.push_back(point);
				for (size_t i = 0; i < 3; ++i)
				{
					lmin[i] = min(lmin[i], point->minimum[i]);
					lmax[i] = max(lmax[i], point->maximum[i]);
				}
			}
			else {
				rbox.push_back(point);
				for (size_t i = 0; i < 3; ++i)
				{
					rmin[i] = min(rmin[i], point->minimum[i]);
					rmax[i] = max(rmax[i], point->maximum[i]);
				}
			}
		}
		if (lbox.size() > 0) left = new BoundingBox(lmin, lmax, lbox);
		if (rbox.size() > 0) right = new BoundingBox(rmin, rmax, rbox);
	}
}

 BoundingBox::~ BoundingBox()
{
	delete left,right;
}