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
	 BoundingBox(size_t l, size_t r, std::vector<RenderPrimitive*> points);
	~BoundingBox();
	virtual bool intersect(Ray const & ray, Hitpoint & hit) const
	{
		//we want to find the farthest entrace and closest exit to the box
		//if the exit is closer than the entrance, there is no hit
		const size_t vecDim = 3;
		float entrance = 0.0f;
		float exit = hit.getParameter();
		Vector3 normal = Vector3(0, 0, 0);

		for (int i = 0; i<vecDim; i++)
		{
			float slabA = minimum[i];
			float slabB = maximum[i];
			float invDir = 1.0f / ray.getDirection()[i];
			float origin = ray.getOrigin()[i];

			float closestHit = (slabA - origin) * invDir;
			float farthestHit = (slabB - origin) * invDir;

			if (farthestHit < closestHit)
				std::swap(closestHit, farthestHit);

			bool tooClose = farthestHit < entrance;
			bool tooFar = closestHit > exit;

			if (tooClose || tooFar)
				return false;

			bool foundNewEntrance = closestHit > entrance;
			entrance = foundNewEntrance ? closestHit : entrance;

			bool foundNewExit = farthestHit < exit;
			exit = foundNewExit ? farthestHit : exit;

			if (foundNewEntrance)
			{
				normal = Vector3(0, 0, 0);
				normal[i] = ray.getDirection()[i] * -1;
				normal.normalize();
			}
		}

		hit.setMaterialId(this->getMaterialId());
		hit.setNormal(normal);
		hit.setParameter(entrance);

		return true;
	}
private:

};
 
 BoundingBox:: BoundingBox(size_t l,size_t r,std::vector<RenderPrimitive*> points)
{
	if (l == r + 1) {
		leaf = points[l];
		minimum = leaf->minimum;
		maximum = leaf->maximum;
	}
	else {
		size_t mid = (l + r) / 2;
		left = new BoundingBox(l, mid,points);
		right = new BoundingBox(mid, r,points);
		minimum = left->minimum;
		maximum = right->maximum;
		for (size_t i = 1; i < 3; ++i)
		{
			float val = maximum[i];
			if (val < minimum[i]) {
				maximum[i] = minimum[i];
				minimum[i] = val;
			}
		}
	}
}

 BoundingBox::~ BoundingBox()
{
	delete left,right;
}