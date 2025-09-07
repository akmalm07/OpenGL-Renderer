#include "headers.h"
#include "physics/include/aabb.h"
#include "physics/include/obb.h"
#include "physics/include/sphere_bound.h"


namespace physics
{
	AABB::AABB(const glm::vec3& min, const glm::vec3& max)
	{
		_minMax.min = min;
		_minMax.max = max;
	}

	MinMax AABB::get_min_max() const
	{
		return _minMax;
	}


	std::unique_ptr<BoundTypeBase> AABB::clone() const
	{
		return std::make_unique<AABB>(*this);
	}

	glType::BoundType AABB::get_bound_type() const
	{
		return glType::BoundType::AABB;
	}

	MinMax AABB::get_aabb_wrap() const
	{
		return _minMax;
	}

	float AABB::get_volume() const
	{
		return (_minMax.max.x - _minMax.min.x) * (_minMax.max.y - _minMax.min.y) * (_minMax.max.z - _minMax.min.z);
	}
	

	CollisionPoint AABB::touching(const AABB& other) const
	{
		return CollisionChecker::aabb_check(*this, other);
	}


	CollisionPoint AABB::touching(const OBB& other) const
	{
		return CollisionChecker::partial_sat_check(other, *this);
	}

	CollisionPoint AABB::touching(const SphereBound& other) const
	{
		return CollisionChecker::sphere_check(other, *this);
	}

	glm::vec3 AABB::get_half_extent() const
	{
		return 0.5f * (_minMax.max - _minMax.min);
	}


	AABB::~AABB() = default;



	void AABB::move(const glm::vec3& offset)
	{
		_minMax += offset;
	}

	glm::vec3 AABB::get_center() const
	{
		return 0.5f * (_minMax.min + _minMax.max);
	}

}

