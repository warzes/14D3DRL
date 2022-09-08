#pragma once

#include "MathCore.h"

struct BoundingBox
{
	Vector3 min;
	Vector3 max;
};

// Check collision between two boxes
// NOTE: Boxes are defined by two points minimum and maximum
inline bool CheckCollisionBoxes(const BoundingBox& box1, const BoundingBox& box2)
{
#if 0
	if ((box1.max.x >= box2.min.x) && (box1.min.x <= box2.max.x))
	{
		if ((box1.max.y < box2.min.y) || (box1.min.y > box2.max.y)) return false;
		if ((box1.max.z < box2.min.z) || (box1.min.z > box2.max.z)) return false;
	}
	else return false;

	return true;
#else
	return box1.min <= box2.max && box1.max >= box2.min;
#endif
}