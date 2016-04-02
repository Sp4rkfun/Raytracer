#pragma once
#include <vector>
#include "RenderPrimitive.h"
#include "Hitpoint.h"
class BVHTree
{
public:
	BVHTree(std::vector<RenderPrimitive*> points);
	~BVHTree();
private:

};

BVHTree::BVHTree(std::vector<RenderPrimitive*> points)
{
	int sz = points.size();
	for (size_t i = 1; i < sz; ++i)
	{
		RenderPrimitive *x = points[i];
		size_t j = i;
		while (j>=0 && points[j]->maximum[0]>x->maximum[0])
		{
			points[j + 1] = points[j];
			--j;
		}
		points[j + 1] = x;
	}
}

BVHTree::~BVHTree()
{
}