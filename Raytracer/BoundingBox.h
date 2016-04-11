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
	 BoundingBox(int l, int r, std::vector<RenderPrimitive*> points);
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
	BoundingBox(0, sz, points);
}
 BoundingBox:: BoundingBox(int l,int r,std::vector<RenderPrimitive*> points)
{
	printf("%d :: %d\n",l,r);
	if (l +1 == r) {
		printf("%d\n", l);
		leaf = points[l];
		minimum = leaf->minimum;
		maximum = leaf->maximum;
	}
	else {
		int mid = (l + r) / 2;
		left = new BoundingBox(l, mid,points);
		right = new BoundingBox(mid, r,points);
		minimum = left->minimum;
		maximum = right->maximum;
		printVec3(minimum);
		printVec3(maximum);
		for (int i = 1; i < 3; ++i)
		{
			minimum[i] = min(left->minimum[i], right->minimum[i]);
			maximum[i] = max(left->maximum[i], right->maximum[i]);
			float val = maximum[i];
		}
		printVec3(minimum);
		printVec3(maximum);
	}
}

 BoundingBox::~ BoundingBox()
{
	delete left,right;
}