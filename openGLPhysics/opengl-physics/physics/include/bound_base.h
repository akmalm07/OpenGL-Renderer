#pragma once

#include "config.h"


namespace physics
{
	struct MinMax
	{
		glm::vec3 min;
		glm::vec3 max;
	};


	class OBB;


	class AABB;


	class SphereBound;


	class BoundTypeBase
	{
	public:
		virtual ~BoundTypeBase() = default;

		virtual std::unique_ptr<BoundTypeBase> clone() const = 0;

		MinMax get_min_max() const;

		glm::vec3 get_center() const;

		float get_volume() const;

		virtual bool is_touching(const SphereBound& other) const = 0;

		virtual bool is_touching(const AABB& other) const = 0;

		virtual bool is_touching(const OBB& other) const = 0;


	protected:
		bool is_touching(const AABB& a, const AABB& b) const;

		bool is_touching(const OBB& a, const OBB& b) const;

		bool is_touching(const AABB& a, const OBB& b) const;



		bool sphere_check(const AABB& a, const AABB& b) const;

		bool sphere_check(const OBB& a, const OBB& b) const;

		bool sphere_check(const AABB& a, const OBB& b) const;


		bool sphere_check(const SphereBound& a, const SphereBound& b) const;

		bool sphere_check(const SphereBound& a, const AABB& b) const;

		bool sphere_check(const SphereBound& a, const OBB& b) const;



		bool full_sat_check(const OBB& a, const OBB& b) const;

		bool full_sat_check(const OBB& a, const AABB& b) const;



		bool aabb_cast_check(const OBB& a, const AABB& b) const;

		bool aabb_cast_check(const OBB& a, const OBB& b) const;


		bool partial_sat_check(const OBB& a, const AABB& b) const;

		float project_extent_along_axis(const glm::mat4& rotationMat, const glm::vec3& halfExtent, const glm::vec3& axis) const;

		float project_extent_along_axis(const std::array<glm::vec3, 3>& axes, const glm::vec3& halfExtent, const glm::vec3& axis) const;

	protected:

		glm::vec3 _center;

		glm::vec3 _min;
		glm::vec3 _max;
	};

}