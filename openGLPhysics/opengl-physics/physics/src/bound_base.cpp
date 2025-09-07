#include "headers.h"
#include "physics/include/bound_base.h"
#include "physics/include/aabb.h"
#include "physics/include/obb.h"
#include "glUtil/include/mesh.h"

#include "physics/include/sphere_bound.h"

namespace physics
{

	CollisionPoint CollisionChecker::aabb_check(const AABB& a, const AABB& b)
	{
		if (a._minMax.max.x >= b._minMax.min.x && a._minMax.min.x <= b._minMax.max.x &&
			a._minMax.max.y >= b._minMax.min.y && a._minMax.min.y <= b._minMax.max.y &&
			a._minMax.max.z >= b._minMax.min.z && a._minMax.min.z <= b._minMax.max.z)
		{
			glm::vec3 overlap{
				std::min(a._minMax.max.x, b._minMax.max.x) - std::max(a._minMax.min.x, b._minMax.min.x),
				std::min(a._minMax.max.y, b._minMax.max.y) - std::max(a._minMax.min.y, b._minMax.min.y),
				std::min(a._minMax.max.z, b._minMax.max.z) - std::max(a._minMax.min.z, b._minMax.min.z)
			};


			int axis = (overlap.x < overlap.y) ? 0 : 1;
			axis = (overlap[axis] < overlap.z) ? axis : 2;

			glm::vec3 normal = glm::vec3(0.0f);
			normal[axis] = ((a._minMax.min[axis] + a._minMax.max[axis] < b._minMax.min[axis] + b._minMax.max[axis]) ? -1.0f : 1.0f);


			return { (glm::max(a._minMax.min, b._minMax.min) + glm::min(a._minMax.max, b._minMax.max)) * 0.5f,
				normal,
				overlap[axis] };
		}
		return {};
	}


	CollisionPoint CollisionChecker::sphere_check(const SphereBound& a, const SphereBound& b)
	{
		if (glm::length(a._center - b._center) < (a._radius + b._radius))
		{
			glm::vec3 direction = glm::normalize(b._center - a._center);
			glm::vec3 contactPoint = a._center + direction * a._radius;
			float penetrationDepth = (a._radius + b._radius) - glm::length(b._center - a._center);
			return { contactPoint, direction, penetrationDepth };
		}
		return {};
	}


	CollisionPoint CollisionChecker::sphere_check(const SphereBound& a, const AABB& b)
	{
		glm::vec3 closestPoint = glm::clamp(a._center, b._minMax.min, b._minMax.max);
		glm::vec3 difference = a._center - closestPoint;
		float distanceSquared = glm::dot(difference, difference);
		if (distanceSquared < (a._radius * a._radius))
		{
			float distance = glm::sqrt(distanceSquared);
			glm::vec3 normal = (distance > 0.0001f) ? (difference / distance) : glm::vec3(1, 0, 0);
			float penetrationDepth = a._radius - distance;
			glm::vec3 contactPoint = closestPoint;
			return { contactPoint, normal, penetrationDepth };
		}
		return {};
	}

	CollisionPoint CollisionChecker::sphere_check(const SphereBound& a, const OBB& b)
	{
		glm::mat3 bRot = glm::toMat3(b._orientation);
		glm::vec3 localCenter = glm::transpose(bRot) * (a._center - b._center);
		glm::vec3 closestPoint = glm::clamp(localCenter, -b._halfExtent, b._halfExtent);
		glm::vec3 difference = localCenter - closestPoint;
		float distanceSquared = glm::dot(difference, difference);
		if (distanceSquared < (a._radius * a._radius))
		{
			float distance = glm::sqrt(distanceSquared);
			glm::vec3 normalLocal = (distance > 0.0001f) ? (difference / distance) : glm::vec3(1, 0, 0);
			glm::vec3 normalWorld = bRot * normalLocal;
			float penetrationDepth = a._radius - distance;
			glm::vec3 contactPoint = b._center + bRot * closestPoint;
			return { contactPoint, normalWorld, penetrationDepth };
		}
		return {};
	}






	CollisionPoint CollisionChecker::full_sat_check(const OBB& a, const OBB& b)
	{
		glm::vec3 centerDiff = b._center - a._center;

		glm::mat3 aRot = glm::toMat3(a._orientation);
		glm::mat3 bRot = glm::toMat3(b._orientation);

		std::array<glm::vec3, 15> testAxes;
		int axisCount = 0;

		for (int i = 0; i < 3; i++) 
			testAxes[axisCount++] = aRot[i];

		for (int i = 0; i < 3; i++) 
			testAxes[axisCount++] = bRot[i];

		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				glm::vec3 cross = glm::cross(aRot[i], bRot[j]);
				if (glm::dot(cross, cross) > 0.0001f) // avoid near-zero axes
					testAxes[axisCount++] = glm::normalize(cross);
			}
		}

		float minPenetration = std::numeric_limits<float>::max();
		glm::vec3 collisionNormal;

		// SAT test
		for (int i = 0; i < axisCount; i++)
		{
			const glm::vec3& axis = testAxes[i];
			if (glm::length2(axis) < 0.00001f) 
				continue; // skip degenerate axes

			// Project both OBBs onto axis
			auto project = [](const glm::vec3& center, const glm::mat3& rot, const glm::vec3& halfExtent, const glm::vec3& axis) {
				float r = 0.0f;
				for (int i = 0; i < 3; i++)
					r += halfExtent[i] * std::abs(glm::dot(rot[i], axis));
				float c = glm::dot(center, axis);
				return std::make_pair(c - r, c + r);
				};

			auto [minA, maxA] = project(a._center, aRot, a._halfExtent, axis);
			auto [minB, maxB] = project(b._center, bRot, b._halfExtent, axis);

			float overlap = std::min(maxA, maxB) - std::max(minA, minB);
			if (overlap <= 0.0f)
			{
				return {};
			}

			if (overlap < minPenetration)
			{
				minPenetration = overlap;
				collisionNormal = axis;

				if (glm::dot(centerDiff, collisionNormal) < 0.0f)
					collisionNormal = -collisionNormal;
			}
		}

		return { (a._center + b._center) * 0.5f, collisionNormal, minPenetration };
	}


	CollisionPoint CollisionChecker::partial_sat_check(const OBB& obb, const AABB& aabb)
	{

		glm::vec3 centerDiff = aabb.get_center() - obb._center;

		glm::mat3 obbRot = glm::toMat3(obb._orientation);
		std::array<glm::vec3, 3> obbAxes = { obbRot[0], obbRot[1], obbRot[2] };

		std::array<glm::vec3, 3> aabbAxes = { glm::vec3(1,0,0), glm::vec3(0,1,0), glm::vec3(0,0,1) };

		std::array<glm::vec3, 15> testAxes;
		int axisCount = 0;

		for (int i = 0; i < 3; i++) 
			testAxes[axisCount++] = obbAxes[i];

		for (int i = 0; i < 3; i++) 
			testAxes[axisCount++] = aabbAxes[i];

		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				glm::vec3 cross = glm::cross(obbAxes[i], aabbAxes[j]);
				if (glm::dot(cross, cross) > 0.0001f)
					testAxes[axisCount++] = glm::normalize(cross);
			}
		}

		float minPenetration = std::numeric_limits<float>::max();
		glm::vec3 collisionNormal;

		auto projectOBB = [&](const glm::vec3& axis) {
			float r = 0.0f;
			for (int i = 0; i < 3; i++)
				r += obb._halfExtent[i] * std::abs(glm::dot(obbAxes[i], axis));
			float c = glm::dot(obb._center, axis);
			return std::make_pair(c - r, c + r);
			};

		auto projectAABB = [&](const glm::vec3& axis) {
			glm::vec3 half = aabb.get_half_extent();
			glm::vec3 center = aabb.get_center();
			float r = half.x * std::abs(glm::dot(glm::vec3(1, 0, 0), axis)) +
				half.y * std::abs(glm::dot(glm::vec3(0, 1, 0), axis)) +
				half.z * std::abs(glm::dot(glm::vec3(0, 0, 1), axis));
			float c = glm::dot(center, axis);
			return std::make_pair(c - r, c + r);
			};

		for (int i = 0; i < axisCount; i++)
		{
			const glm::vec3& axis = testAxes[i];
			if (glm::length2(axis) < 0.00001f) 
				continue;

			auto [minO, maxO] = projectOBB(axis);
			auto [minA, maxA] = projectAABB(axis);

			float overlap = std::min(maxO, maxA) - std::max(minO, minA);
			if (overlap <= 0.0f)
			{
				return {};
			}

			if (overlap < minPenetration)
			{
				minPenetration = overlap;
				collisionNormal = axis;
				if (glm::dot(centerDiff, collisionNormal) < 0.0f)
					collisionNormal = -collisionNormal;
			}
		}

		return { (obb._center + aabb.get_center()) * 0.5f, collisionNormal, minPenetration };
	}


	CollisionPoint BoundTypeBase::touching(BoundTypeBase* other) const
	{
		switch (other->get_bound_type())
		{
		case glType::BoundType::AABB:
			return touching(*static_cast<AABB*>(other));
			break;
		case glType::BoundType::OBB:
			return touching(*static_cast<OBB*>(other));
			break;
		case glType::BoundType::Sphere:
			return touching(*static_cast<SphereBound*>(other));
			break;
		default:
			return {};
			break;
		}
	}

	float project_extent_along_axis(const glm::mat4& rotationMat, const glm::vec3& halfExtent, const glm::vec3& axis)
	{
		std::array<glm::vec3, 3> _OBBAxes = {
			rotationMat[0],
			rotationMat[1],
			rotationMat[2]
		};

		return  std::abs(glm::dot(_OBBAxes[0], axis)) * halfExtent.x +
			std::abs(glm::dot(_OBBAxes[1], axis)) * halfExtent.y +
			std::abs(glm::dot(_OBBAxes[2], axis)) * halfExtent.z;
	}

	float project_extent_along_axis(const std::array<glm::vec3, 3>& axes, const glm::vec3& halfExtent, const glm::vec3& axis)
	{
		return std::abs(glm::dot(axes[0], axis)) * halfExtent.x +
			std::abs(glm::dot(axes[1], axis)) * halfExtent.y +
			std::abs(glm::dot(axes[2], axis)) * halfExtent.z;
	}


	glm::vec3 find_center_of_mass(std::vector<glType::Vertex> verts, glUtil::FullStride fullStride, glUtil::PosStride posStride)
	{
		if (verts.empty())
		{
			return glm::vec3(0.0f);
		}
		std::vector<glm::vec3> vertices;
		for (size_t i = 0; i < verts.size(); i += static_cast<size_t>(fullStride))
		{
			glm::vec3 vertex;
			vertex.x = verts[i];
			vertex.y = verts[i + 1];
			if (posStride == glUtil::PosStride::STRIDE_3D)
			{
				vertex.z = verts[i + 2];
			}
			else if (posStride == glUtil::PosStride::STRIDE_2D)
			{
				vertex.z = 0.0f;
			}
			vertices.push_back(vertex);
		}
		glm::vec3 centerOfMass(0.0f);
		for (const auto& vertex : vertices)
		{
			centerOfMass += vertex;
		}
		centerOfMass /= vertices.size();
		return centerOfMass;
	}

	glm::vec3 find_center_of_mass(std::vector<glm::vec3> verts)
	{
		if (verts.empty())
		{
			return glm::vec3(0.0f);
		}
		
		glm::vec3 centerOfMass(0.0f);
	
		for (const auto& vertex : verts)
		{
			centerOfMass += vertex;
		}
		centerOfMass /= verts.size();

		return centerOfMass;
	}

	MinMax get_min_max_from_vertices(const std::vector<glType::Vertex>& verts, glUtil::FullStride fullStride, glUtil::PosStride posStride)
	{

		if (verts.empty())
		{
			return { glm::vec3(0.0f), glm::vec3(0.0f) };
		}

		std::vector<glm::vec3> vertices;

		for (size_t i = 0; i < verts.size(); i += static_cast<size_t>(fullStride))
		{

			glm::vec3 vertex;


			vertex.x = verts[i];
			vertex.y = verts[i + 1];

			if (posStride == glUtil::PosStride::STRIDE_3D)
			{
				vertex.z = verts[i + 2];
			}
			else if (posStride == glUtil::PosStride::STRIDE_2D)
			{
				vertex.z = 0.0f;
			}

			vertices.push_back(vertex);
		}

		glm::vec3 minVec = vertices[0];
		glm::vec3 maxVec = vertices[0];

		for (const auto& vertex : vertices)
		{
			minVec = glm::min(minVec, vertex);
			maxVec = glm::max(maxVec, vertex);
		}

		return { minVec, maxVec };
	}

	MinMax get_min_max_of_mesh(const glUtil::Mesh& mesh, glUtil::FullStride fullStride, glUtil::PosStride posStride)
	{
		auto vert = mesh.get_verticies();
		if (vert.empty())
		{
			return { glm::vec3(0.0f), glm::vec3(0.0f) };
		}
		return get_min_max_from_vertices(vert, fullStride, posStride);
	}

} // namespace physics