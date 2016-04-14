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
		const Vector3 &dir = ray.dir;
		const Vector3 &orig = ray.origin;
		Vector3 invDir(1.0f / dir[0], 1.0f / dir[1], 1.0f / dir[2]);


		float x1 = (minimum[0] - orig[0])*invDir[0];
		float x2 = (maximum[0] - orig[0])*invDir[0];
		float tmin = min(x1, x2);
		float tmax = max(x1, x2);
		float x3 = (minimum[1] - orig[1])*invDir[1];
		float x4 = (maximum[1] - orig[1])*invDir[1];
		tmin = max(tmin, min(x3, x4));
		tmax = min(tmax, max(x3, x4));
		float x5 = (minimum[2] - orig[2])*invDir[2];
		float x6 = (maximum[2] - orig[2])*invDir[2];
		tmin = max(tmin, min(x5, x6));
		tmax = min(tmax, max(x5, x6));
		if (tmax < 0||tmin>tmax)return -1.0f;
		else return tmax;

	}
	RenderPrimitive *getIntersect(Ray const & ray) {
		return recurIntersect(ray,intersect(ray));
	}
	RenderPrimitive *recurIntersect(Ray const &ray,float dist) {
		if (dist > -1) {
			//printf("recursing\n");
			if (leaf != nullptr) {
				if(leaf->intersects(ray)>-1)
				return leaf;
				else return nullptr;
			}
			float ld = left->intersect(ray);
			float rd = right->intersect(ray);
			if (ld == -1)return right->recurIntersect(ray, rd);
			else if (rd == -1)return left->recurIntersect(ray, ld);
			else if (ld == -1 && rd == -1)return nullptr;
			else {
				RenderPrimitive *l = left->recurIntersect(ray, ld);
				RenderPrimitive *r = right->recurIntersect(ray, rd);
				if (l == nullptr)return r;
				else if (r == nullptr)return l;
				ld = l->intersects(ray);
				rd = r->intersects(ray);
				/*if (ld == -1 && rd == -1)return nullptr;
				else if (ld == -1)return r;
				else if (rd == -1)return l;*/
				if (ld < rd)return l;
				else return r;
			}
		}
		else
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
BoundingBox::BoundingBox(Vector3 minimum, Vector3 maximum, std::vector<RenderPrimitive*> points):minimum(minimum),maximum(maximum)
{
	//printf("%d\n",points.size());
	int sz = points.size();
	if (sz == 1) { leaf = points[0];}
	else {
		Vector3 difference = maximum - minimum;
		int axis = difference.maxComponent();
		std::vector<RenderPrimitive*> lbox, rbox;
		Vector3 lmin(FLT_MAX, FLT_MAX, FLT_MAX), lmax(-FLT_MAX, -FLT_MAX, -FLT_MAX),
				rmin(FLT_MAX, FLT_MAX, FLT_MAX), rmax(-FLT_MAX, -FLT_MAX, -FLT_MAX);

		float mid = minimum[axis] + (difference[axis] / 2.0f);
		for (size_t i = 0; i < sz; i++)
		{
			RenderPrimitive *point = points[i];
			if (point->midpoint[axis] < mid) {
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
		if (lbox.size() == points.size()) { rbox.push_back(lbox.back()); lbox.pop_back(); }
		else if (rbox.size() == points.size()) { lbox.push_back(rbox.back()); rbox.pop_back(); }
		if (lbox.size() > 0) left = new BoundingBox(lmin, lmax, lbox);
		if (rbox.size() > 0) right = new BoundingBox(rmin, rmax, rbox);
	}
}

 BoundingBox::~ BoundingBox()
{
	delete left,right;
}