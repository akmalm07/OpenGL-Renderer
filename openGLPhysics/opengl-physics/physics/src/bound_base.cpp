#include "headers.h"
#include "physics/include/bound_base.h"
#include "physics/include/aabb.h"
#include "physics/include/obb.h"

#include "physics/include/sphere_bound.h"

namespace physics
{


	MinMax BoundTypeBase::get_min_max() const
	{
		return { _min, _max };
	}

	bool BoundTypeBase::is_touching(const AABB& a, const AABB& b) const
	{
		return !(
			a._max.x < b._min.x || a._min.x > b._max.x ||
			a._max.y < b._min.y || a._min.y > b._max.y ||
			a._max.z < b._min.z || a._min.z > b._max.z);
	}



	bool BoundTypeBase::is_touching(const OBB& a, const OBB& b) const
	{
		if (BoundTypeBase::sphere_check(a, b) || BoundTypeBase::aabb_cast_check(b, a))
		{
			return full_sat_check(a, b);
		}
		else
		{
			return false;
		}
	}


	bool BoundTypeBase::is_touching(const AABB& a, const OBB& b) const
	{
		if (BoundTypeBase::sphere_check(a, b) || BoundTypeBase::partial_sat_check(b, a))
		{
			return full_sat_check(b, a);
		}
		else
		{
			return false;
		}
	}


	bool BoundTypeBase::sphere_check(const AABB& a, const AABB& b) const
	{
		return	glm::length(a._center - b._center) < // Distance
			(
				glm::length(0.5f * (a._max - a._min) + // radiusA
					glm::length(0.5f * (b._max - b._min))) // radiusB
				);
	}


	bool BoundTypeBase::sphere_check(const OBB& a, const OBB& b) const
	{
		return	glm::length(a._center - b._center) < // Distance
			(
				glm::length(0.5f * (a._max - a._min) + // radiusA
					glm::length(0.5f * (b._max - b._min))) // radiusB
				);
	}


	bool BoundTypeBase::sphere_check(const AABB& a, const OBB& b) const
	{
		return	glm::length(a._center - b._center) < // Distance
			(
				glm::length(0.5f * (a._max - a._min) + // radiusA
					glm::length(0.5f * (b._max - b._min))) // radiusB
				);
	}

	bool BoundTypeBase::sphere_check(const SphereBound& a, const SphereBound& b) const
	{
		return glm::length(a._center - b._center) < // Distance
			(a._radius + b._radius); // radiusA + radiusB
	}

	bool BoundTypeBase::sphere_check(const SphereBound& a, const AABB& b) const
	{
		return glm::length(a._center - b._center) < // Distance
			(a._radius + glm::length(0.5f * (b._max - b._min))); // radiusA + radiusB
	}

	bool BoundTypeBase::sphere_check(const SphereBound& a, const OBB& b) const
	{
		return glm::length(a._center - b._center) < // Distance
			(a._radius + glm::length(0.5f * (b._max - b._min))); // radiusA + radiusB
	}


	bool BoundTypeBase::full_sat_check(const OBB& a, const OBB& b) const
	{
		glm::vec3 centerDiff = b._center - a._center;

		glm::mat4 aRotationMat = a.get_rotation_matrix();
		std::array<glm::vec3, 3> axisA = {
			glm::vec3(aRotationMat[0]),
			glm::vec3(aRotationMat[1]),
			glm::vec3(aRotationMat[2])
		};

		glm::mat4 bRotationMat = b.get_rotation_matrix();
		std::array<glm::vec3, 3> axisB = {
			glm::vec3(bRotationMat[0]),
			glm::vec3(bRotationMat[1]),
			glm::vec3(bRotationMat[2])
		};

		std::array<glm::vec3, 15> testAxes;

		int axisCount = 0;

		for (int i = 0; i < 3; i++)
		{
			testAxes[axisCount++] = axisA[i];
		}

		for (int i = 0; i < 3; i++)
		{
			testAxes[axisCount++] = axisB[i];
		}

		for (const auto& axis1 : axisA)
		{
			for (const auto& axis2 : axisB)
			{
				glm::vec3 cross = glm::cross(axis1, axis2);
				float lengthSquared = glm::dot(cross, cross);

				if (lengthSquared > 0.0001f)
				{
					testAxes[axisCount++] = cross;
				}
			}
		}

		glm::vec3 aHalfExtent = a._halfExtent;
		glm::vec3 bHalfExtent = b._halfExtent;

		for (int i = 0; i < axisCount; i++)
		{
			const glm::vec3& axis = testAxes[i];

			float projA = project_extent_along_axis(aRotationMat, aHalfExtent, axis);
			float projB = project_extent_along_axis(bRotationMat, bHalfExtent, axis);

			float distance = std::abs(glm::dot(centerDiff, axis));

			if (distance > projA + projB)
				return false;
		}

		return true;
	}


	bool BoundTypeBase::full_sat_check(const OBB& a, const AABB& b) const
	{
		glm::vec3 centerDiff = b._center - a._center;

		std::array<glm::vec3, 3> axisA = {
			glm::vec3(a.get_rotation_matrix()[0]),
			glm::vec3(a.get_rotation_matrix()[1]),
			glm::vec3(a.get_rotation_matrix()[2])
		};

		std::array<glm::vec3, 3> axisB = {
			glm::vec3(1, 0, 0),
			glm::vec3(0, 1, 0),
			glm::vec3(0, 0, 1)
		};

		std::array<glm::vec3, 15> testAxes{};
		int axisCount = 0;

		for (int i = 0; i < 3; i++)
			testAxes[axisCount++] = axisA[i];

		for (int i = 0; i < 3; i++)
			testAxes[axisCount++] = axisB[i];

		for (const auto& axis1 : axisA)
		{
			for (const auto& axis2 : axisB)
			{
				glm::vec3 cross = glm::cross(axis1, axis2);
				float lengthSquared = glm::dot(cross, cross);

				if (lengthSquared > 0.0001f)
				{
					testAxes[axisCount++] = glm::normalize(cross);
				}
			}
		}

		const glm::vec3 aHalf = a._halfExtent;
		const glm::vec3 bHalf = b._halfExtent;

		for (int i = 0; i < axisCount; i++)
		{
			const glm::vec3& axis = testAxes[i];
			float projA = project_extent_along_axis(axisA, aHalf, axis);
			float projB = project_extent_along_axis(axisB, bHalf, axis);
			float distance = std::abs(glm::dot(centerDiff, axis));

			if (distance > projA + projB)
				return false;
		}

		return true;
	}


	bool BoundTypeBase::aabb_cast_check(const OBB& a, const AABB& b) const
	{
		const glm::vec3& aMin = a.get_aabb_wrap_min();
		const glm::vec3& aMax = a.get_aabb_wrap_max();

		const glm::vec3& bMin = b.get_min();
		const glm::vec3& bMax = b.get_max();

		return (aMin.x <= bMax.x && aMax.x >= bMin.x) &&
			(aMin.y <= bMax.y && aMax.y >= bMin.y) &&
			(aMin.z <= bMax.z && aMax.z >= bMin.z);
	}


	bool BoundTypeBase::aabb_cast_check(const OBB& a, const OBB& b) const
	{
		const glm::vec3& aMin = a.get_aabb_wrap_min();
		const glm::vec3& aMax = a.get_aabb_wrap_max();

		const glm::vec3& bMin = b.get_aabb_wrap_min();
		const glm::vec3& bMax = b.get_aabb_wrap_max();

		return (aMin.x <= bMax.x && aMax.x >= bMin.x) &&
			(aMin.y <= bMax.y && aMax.y >= bMin.y) &&
			(aMin.z <= bMax.z && aMax.z >= bMin.z);
	}


	bool BoundTypeBase::partial_sat_check(const OBB& a, const AABB& b) const
	{

		glm::vec3 centerDiff = b._center - a._center;

		std::array<glm::vec3, 3> axisA = {
			glm::vec3(a.get_rotation_matrix()[0]),
			glm::vec3(a.get_rotation_matrix()[1]),
			glm::vec3(a.get_rotation_matrix()[2])
		};

		std::array<glm::vec3, 3> axisB = {
			glm::vec3(1, 0, 0),
			glm::vec3(0, 1, 0),
			glm::vec3(0, 0, 1)
		};

		std::array<glm::vec3, 6> testAxes;
		int axisCount = 0;

		for (int i = 0; i < 3; i++)
		{
			testAxes[axisCount++] = axisA[i];
		}

		for (int i = 0; i < 3; i++)
		{
			testAxes[axisCount++] = axisB[i];
		}

		glm::vec3 aHalfExtent = a._halfExtent;
		glm::vec3 bHalfExtent = b._halfExtent;

		for (int i = 0; i < axisCount; i++)
		{
			const glm::vec3& axis = testAxes[i];

			float projA = project_extent_along_axis(axisA, aHalfExtent, axis);
			float projB = project_extent_along_axis(axisB, bHalfExtent, axis);

			float distance = std::abs(glm::dot(centerDiff, axis));

			if (distance > projA + projB)
				return false;
		}

		return true;

	}

	glm::vec3 BoundTypeBase::get_center() const
	{
		return _center;
	}

	float BoundTypeBase::get_volume() const
	{
		return (_max.x - _min.x) * (_max.y - _min.y) * (_max.z - _min.z);
	}

	glm::vec3 AABB::get_half_extent() const
	{
		return _halfExtent;
	}

	float BoundTypeBase::project_extent_along_axis(const glm::mat4& rotationMat, const glm::vec3& halfExtent, const glm::vec3& axis) const
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

	float BoundTypeBase::project_extent_along_axis(const std::array<glm::vec3, 3>& axes, const glm::vec3& halfExtent, const glm::vec3& axis) const
	{
		return std::abs(glm::dot(axes[0], axis)) * halfExtent.x +
			std::abs(glm::dot(axes[1], axis)) * halfExtent.y +
			std::abs(glm::dot(axes[2], axis)) * halfExtent.z;
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

} // namespace physics