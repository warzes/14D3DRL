#pragma once

#include "MathCore.h"

struct Plane
{
	Plane() = default;
	Plane(const glm::vec3& p1, const glm::vec3& norm) : normal(glm::normalize(norm)), distance(glm::dot(normal, p1)) {}

	float GetSignedDistanceToPlan(const glm::vec3& point) const
	{
		return glm::dot(normal, point) - distance;
	}

	glm::vec3 normal = { 0.0f, 1.0f, 0.0f }; // unit vector
	float distance = 0.0f;                   // Distance with origin
};

struct Frustum
{
	Plane topFace;
	Plane bottomFace;

	Plane rightFace;
	Plane leftFace;

	Plane farFace;
	Plane nearFace;
};


//struct AABB
//{
//	AABB(const glm::vec3& min, const glm::vec3& max)  : center{ (max + min) * 0.5f }, extents{ max.x - center.x, max.y - center.y, max.z - center.z } {}
//	AABB(const glm::vec3& inCenter, float iI, float iJ, float iK) : center{ inCenter }, extents{ iI, iJ, iK } {}
//
//    bool isOnFrustum(const Frustum& camFrustum, const Transform& transform) const
//    {
//        //Get global scale thanks to our transform
//        const glm::vec3 globalCenter{ transform.getModelMatrix() * glm::vec4(center, 1.f) };
//
//        // Scaled orientation
//        const glm::vec3 right = transform.getRight() * extents.x;
//        const glm::vec3 up = transform.getUp() * extents.y;
//        const glm::vec3 forward = transform.getForward() * extents.z;
//
//        const float newIi = std::abs(glm::dot(glm::vec3{ 1.f, 0.f, 0.f }, right)) +
//            std::abs(glm::dot(glm::vec3{ 1.f, 0.f, 0.f }, up)) +
//            std::abs(glm::dot(glm::vec3{ 1.f, 0.f, 0.f }, forward));
//
//        const float newIj = std::abs(glm::dot(glm::vec3{ 0.f, 1.f, 0.f }, right)) +
//            std::abs(glm::dot(glm::vec3{ 0.f, 1.f, 0.f }, up)) +
//            std::abs(glm::dot(glm::vec3{ 0.f, 1.f, 0.f }, forward));
//
//        const float newIk = std::abs(glm::dot(glm::vec3{ 0.f, 0.f, 1.f }, right)) +
//            std::abs(glm::dot(glm::vec3{ 0.f, 0.f, 1.f }, up)) +
//            std::abs(glm::dot(glm::vec3{ 0.f, 0.f, 1.f }, forward));
//
//        //We not need to divise scale because it's based on the half extention of the AABB
//        const AABB globalAABB(globalCenter, newIi, newIj, newIk);
//
//        return (globalAABB.isOnOrForwardPlan(camFrustum.leftFace) &&
//            globalAABB.isOnOrForwardPlan(camFrustum.rightFace) &&
//            globalAABB.isOnOrForwardPlan(camFrustum.topFace) &&
//            globalAABB.isOnOrForwardPlan(camFrustum.bottomFace) &&
//            globalAABB.isOnOrForwardPlan(camFrustum.nearFace) &&
//            globalAABB.isOnOrForwardPlan(camFrustum.farFace));
//    };
//
//
//    bool isOnOrForwardPlan(const Plane& plan) const
//    {
//        // Compute the projection interval radius of b onto L(t) = b.c + t * p.n
//        const float r = extents.x * std::abs(plan.normal.x) +
//            extents.y * std::abs(plan.normal.y) + extents.z * std::abs(plan.normal.z);
//
//        return -r <= plan.GetSignedDistanceToPlan(center);
//    }
//
//	glm::vec3 center{ 0.f, 0.f, 0.f };
//	glm::vec3 extents{ 0.f, 0.f, 0.f };
//};

inline void NormalizePlane(Vector4& plane)
{
	float magnitude = sqrtf(plane.x * plane.x + plane.y * plane.y + plane.z * plane.z);
	plane.x /= magnitude;
	plane.y /= magnitude;
	plane.z /= magnitude;
	plane.w /= magnitude;
}

inline float DistanceToPlane(const Vector4& plane, const Vector3& position)
{
	return (plane.x * position.x + plane.y * position.y + plane.z * position.z + plane.w);
}

inline float DistanceToPlane(const Vector4& plane, float x, float y, float z)
{
	return (plane.x * x + plane.y * y + plane.z * z + plane.w);
}

class SimpleFrustum
{
public:
	enum class FrustumPlanes
	{
		Back = 0,
		Front = 1,
		Bottom = 2,
		Top = 3,
		Right = 4,
		Left = 5,
		MAX = 6
	};

	SimpleFrustum()
	{
		Planes[static_cast<size_t>(FrustumPlanes::Right)] = Vector4{ 0 };
		Planes[static_cast<size_t>(FrustumPlanes::Left)] = Vector4{ 0 };
		Planes[static_cast<size_t>(FrustumPlanes::Top)] = Vector4{ 0 };
		Planes[static_cast<size_t>(FrustumPlanes::Bottom)] = Vector4{ 0 };
		Planes[static_cast<size_t>(FrustumPlanes::Front)] = Vector4{ 0 };
		Planes[static_cast<size_t>(FrustumPlanes::Back)] = Vector4{ 0 };
	}

	void Extract(const glm::mat4& projection, const glm::mat4& modelview)
	{
		Matrix4 planes;

		planes[0] = modelview[0][0] * projection[0][0] + modelview[0][1] * projection[1][0] + modelview[0][2] * projection[2][0] + modelview[0][3] * projection[3][0];
		planes[1] = modelview[0][0] * projection[0][1] + modelview[0][1] * projection[1][1] + modelview[0][2] * projection[2][1] + modelview[0][3] * projection[3][1];
		planes[2] = modelview[0][0] * projection[0][2] + modelview[0][1] * projection[1][2] + modelview[0][2] * projection[2][2] + modelview[0][3] * projection[3][2];
		planes[3] = modelview[0][0] * projection[0][3] + modelview[0][1] * projection[1][3] + modelview[0][2] * projection[2][3] + modelview[0][3] * projection[3][3];
		planes[4] = modelview[1][0] * projection[0][0] + modelview[1][1] * projection[1][0] + modelview[1][2] * projection[2][0] + modelview[1][3] * projection[3][0];
		planes[5] = modelview[1][0] * projection[0][1] + modelview[1][1] * projection[1][1] + modelview[1][2] * projection[2][1] + modelview[1][3] * projection[3][1];
		planes[6] = modelview[1][0] * projection[0][2] + modelview[1][1] * projection[1][2] + modelview[1][2] * projection[2][2] + modelview[1][3] * projection[3][2];
		planes[7] = modelview[1][0] * projection[0][3] + modelview[1][1] * projection[1][3] + modelview[1][2] * projection[2][3] + modelview[1][3] * projection[3][3];
		planes[8] = modelview[2][0] * projection[0][0] + modelview[2][1] * projection[1][0] + modelview[2][2] * projection[2][0] + modelview[2][3] * projection[3][0];
		planes[9] = modelview[2][0] * projection[0][1] + modelview[2][1] * projection[1][1] + modelview[2][2] * projection[2][1] + modelview[2][3] * projection[3][1];
		planes[10] = modelview[2][0] * projection[0][2] + modelview[2][1] * projection[1][2] + modelview[2][2] * projection[2][2] + modelview[2][3] * projection[3][2];
		planes[11] = modelview[2][0] * projection[0][3] + modelview[2][1] * projection[1][3] + modelview[2][2] * projection[2][3] + modelview[2][3] * projection[3][3];
		planes[12] = modelview[3][0] * projection[0][0] + modelview[3][1] * projection[1][0] + modelview[3][2] * projection[2][0] + modelview[3][3] * projection[3][0];
		planes[13] = modelview[3][0] * projection[0][1] + modelview[3][1] * projection[1][1] + modelview[3][2] * projection[2][1] + modelview[3][3] * projection[3][1];
		planes[14] = modelview[3][0] * projection[0][2] + modelview[3][1] * projection[1][2] + modelview[3][2] * projection[2][2] + modelview[3][3] * projection[3][2];
		planes[15] = modelview[3][0] * projection[0][3] + modelview[3][1] * projection[1][3] + modelview[3][2] * projection[2][3] + modelview[3][3] * projection[3][3];

		Planes[static_cast<size_t>(FrustumPlanes::Right)] = { (float)planes[3] - planes[0], planes[7] - planes[4], planes[11] - planes[8], planes[15] - planes[12] };
		NormalizePlane(Planes[static_cast<size_t>(FrustumPlanes::Right)]);

		Planes[static_cast<size_t>(FrustumPlanes::Left)] = { planes[3] + planes[0], planes[7] + planes[4], planes[11] + planes[8], planes[15] + planes[12] };
		NormalizePlane(Planes[static_cast<size_t>(FrustumPlanes::Left)]);

		Planes[static_cast<size_t>(FrustumPlanes::Top)] = { planes[3] - planes[1], planes[7] - planes[5], planes[11] - planes[9], planes[15] - planes[13] };
		NormalizePlane(Planes[static_cast<size_t>(FrustumPlanes::Top)]);

		Planes[static_cast<size_t>(FrustumPlanes::Bottom)] = { planes[3] + planes[1], planes[7] + planes[5], planes[11] + planes[9], planes[15] + planes[13] };
		NormalizePlane(Planes[static_cast<size_t>(FrustumPlanes::Bottom)]);

		Planes[static_cast<size_t>(FrustumPlanes::Back)] = { planes[3] - planes[2], planes[7] - planes[6], planes[11] - planes[10], planes[15] - planes[14] };
		NormalizePlane(Planes[static_cast<size_t>(FrustumPlanes::Back)]);

		Planes[static_cast<size_t>(FrustumPlanes::Front)] = { planes[3] + planes[2], planes[7] + planes[6], planes[11] + planes[10], planes[15] + planes[14] };
		NormalizePlane(Planes[static_cast<size_t>(FrustumPlanes::Front)]);
	}

	bool PointIn(const Vector3& position) const 
	{
		for (auto& plane : Planes)
		{
			if (DistanceToPlane(plane, position) <= 0) // point is behind plane
				return false;
		}

		return true;
	}
	bool PointIn(float x, float y, float z) const 
	{
		for (auto& plane : Planes)
		{
			if (DistanceToPlane(plane, x, y, z) <= 0) // point is behind plane
				return false;
		}

		return true;
	}

	bool SphereIn(const Vector3& position, float radius) const
	{
		for (auto& plane : Planes)
		{
			if (DistanceToPlane(plane, position) < -radius) // center is behind plane by more than the radius
				return false;
		}

		return true;
	}
	bool AABBoxIn(const Vector3& min, const Vector3& max) const
	{
		// if any point is in and we are good
		if (PointIn(min.x, min.y, min.z))
			return true;

		if (PointIn(min.x, max.y, min.z))
			return true;

		if (PointIn(max.x, max.y, min.z))
			return true;

		if (PointIn(max.x, min.y, min.z))
			return true;

		if (PointIn(min.x, min.y, max.z))
			return true;

		if (PointIn(min.x, max.y, max.z))
			return true;

		if (PointIn(max.x, max.y, max.z))
			return true;

		if (PointIn(max.x, min.y, max.z))
			return true;

		// check to see if all points are outside of any one plane, if so the entire box is outside
		for (auto& plane : Planes)
		{
			bool oneInside = false;

			if (DistanceToPlane(plane, min.x, min.y, min.z) >= 0)
				oneInside = true;

			if (DistanceToPlane(plane, max.x, min.y, min.z) >= 0)
				oneInside = true;

			if (DistanceToPlane(plane, max.x, max.y, min.z) >= 0)
				oneInside = true;

			if (DistanceToPlane(plane, min.x, max.y, min.z) >= 0)
				oneInside = true;

			if (DistanceToPlane(plane, min.x, min.y, max.z) >= 0)
				oneInside = true;

			if (DistanceToPlane(plane, max.x, min.y, max.z) >= 0)
				oneInside = true;

			if (DistanceToPlane(plane, max.x, max.y, max.z) >= 0)
				oneInside = true;

			if (DistanceToPlane(plane, min.x, max.y, max.z) >= 0)
				oneInside = true;

			if (!oneInside)
				return false;
		}

		// the box extends outside the frustum but crosses it
		return true;
	}

	Vector4 Planes[6];
};