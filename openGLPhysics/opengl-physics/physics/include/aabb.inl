#pragma once

#include "physics/include/aabb.h"

namespace physics
{

	template<_PRECALC_COORDS T>
	inline bool _CHECK_COLLISION::is_touching(const _AABB<T>& a, const _AABB<T>& b) const
	{
		return !(
			a._max.x < b._min.x || a._min.x > b._max.x ||
			a._max.y < b._min.y || a._min.y > b._max.y ||
			a._max.z < b._min.z || a._min.z > b._max.z);
	}


	template<_PRECALC_COORDS T>
	inline bool _CHECK_COLLISION::is_touching(const _OBB<T>& a, const _OBB<T>& b) const
	{
		if (_CHECK_COLLISION::sphere_check(a, b) || _CHECK_COLLISION::_AABB_cast_check(b, a))
		{
			return full_sat_check(a, b);
		}
		else
		{
			return false;
		}
	}

	template<_PRECALC_COORDS T>
	inline bool _CHECK_COLLISION::is_touching(const _AABB<T>& a, const _OBB<T>& b) const
	{
		if (_CHECK_COLLISION::sphere_check(a, b) || _CHECK_COLLISION::partial_sat_check(b, a))
		{
			return full_sat_check(b, a);
		}
		else
		{
			return false;
		}
	}

	template<_PRECALC_COORDS T>
	inline bool _CHECK_COLLISION::sphere_check(const _AABB<T>& a, const _AABB<T>& b) const
	{
		return	glm::length(a._center - b._center) < // Distance
			(
				glm::length(0.5f * (a._max - a._min) + // radiusA
				glm::length(0.5f * (b._max - b._min))) // radiusB
			);
	}

	template<_PRECALC_COORDS T>
	inline bool _CHECK_COLLISION::sphere_check(const _OBB<T>& a, const _OBB<T>& b) const
	{
		return	glm::length(a._center - b._center) < // Distance
			(
				glm::length(0.5f * (a._max - a._min) + // radiusA
				glm::length(0.5f * (b._max - b._min))) // radiusB
			);
	}

	template<_PRECALC_COORDS T>
	inline bool _CHECK_COLLISION::sphere_check(const _AABB<T>& a, const _OBB<T>& b) const
	{
		return	glm::length(a._center - b._center) < // Distance
			(
				glm::length(0.5f * (a._max - a._min) + // radiusA
				glm::length(0.5f * (b._max - b._min))) // radiusB
			);
	}

	template<_PRECALC_COORDS T>
	inline bool _CHECK_COLLISION::full_sat_check(const _OBB<T>& a, const _OBB<T>& b) const
	{
		glm::vec3 centerDiff = b.get_center() - a.get_center();

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

		glm::vec3 aHalfExtent = a.get_half_extent();
		glm::vec3 bHalfExtent = b.get_half_extent();

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

	template<_PRECALC_COORDS T>
	inline bool _CHECK_COLLISION::full_sat_check(const _OBB<T>& a, const _AABB<T>& b) const
	{
		glm::vec3 centerDiff = b.get_center() - a.get_center();

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

		const glm::vec3 aHalf = a.get_half_extent();
		const glm::vec3 bHalf = b.get_half_extent();

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

	template<_PRECALC_COORDS T>
	inline bool _CHECK_COLLISION::_AABB_cast_check(const _OBB<T>& a, const _AABB<T>& b) const
	{
		const glm::vec3& aMin = a.get__AABB_wrap_min();
		const glm::vec3& aMax = a.get__AABB_wrap_max();

		const glm::vec3& bMin = b.get_min();
		const glm::vec3& bMax = b.get_max();

		return (aMin.x <= bMax.x && aMax.x >= bMin.x) &&
			(aMin.y <= bMax.y && aMax.y >= bMin.y) &&
			(aMin.z <= bMax.z && aMax.z >= bMin.z);
	}

	template<_PRECALC_COORDS T>
	inline bool _CHECK_COLLISION::_AABB_cast_check(const _OBB<T>& a, const _OBB<T>& b) const
	{
		const glm::vec3& aMin = a.get__AABB_wrap_min();
		const glm::vec3& aMax = a.get__AABB_wrap_max();

		const glm::vec3& bMin = b.get__AABB_wrap_min();
		const glm::vec3& bMax = b.get__AABB_wrap_max();

		return (aMin.x <= bMax.x && aMax.x >= bMin.x) &&
			(aMin.y <= bMax.y && aMax.y >= bMin.y) &&
			(aMin.z <= bMax.z && aMax.z >= bMin.z);
	}

	template<_PRECALC_COORDS T>
	inline bool _CHECK_COLLISION::partial_sat_check(const _OBB<T>& a, const _AABB<T>& b) const
	{
		if (!(10.0f <= a.get_rotation_degree() <= -10.0f))
		{
			return false;
		}

		glm::vec3 centerDiff = b.get_center() - a.get_center();

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

		glm::vec3 aHalfExtent = a.get_half_extent();
		glm::vec3 bHalfExtent = b.get_half_extent();

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

	inline float _CHECK_COLLISION::project_extent_along_axis(const glm::mat4& rotationMat, const glm::vec3& halfExtent, const glm::vec3& axis) const
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

	inline float _CHECK_COLLISION::project_extent_along_axis(const std::array<glm::vec3, 3>& axes, const glm::vec3& halfExtent, const glm::vec3& axis) const
	{
		return std::abs(glm::dot(axes[0], axis)) * halfExtent.x +
			std::abs(glm::dot(axes[1], axis)) * halfExtent.y +
			std::abs(glm::dot(axes[2], axis)) * halfExtent.z;
	}


	template<_PRECALC_COORDS T>
	bool _AABB <_PRECALC_COORDS::YES>::is_touching(const _AABB<T>& other) const
	{
		return _CHECK_COLLISION::is_touching(*this, other);
	}

	template<_PRECALC_COORDS T>
	bool _AABB <_PRECALC_COORDS::YES>::is_touching(const _OBB<T>& other) const
	{
		return _CHECK_COLLISION::is_touching(*this, other);
	}


	template<_PRECALC_COORDS T>
	bool _AABB <_PRECALC_COORDS::NO>::is_touching(const _AABB<T>& other) const
	{
		return _CHECK_COLLISION::is_touching(*this, other);
	}

	template<_PRECALC_COORDS T>
	bool _AABB <_PRECALC_COORDS::NO>::is_touching(const _OBB<T>& other) const
	{
		return _CHECK_COLLISION::is_touching(*this, other);
	}

} // namespace physics