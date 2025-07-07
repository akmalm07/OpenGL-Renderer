#include "headers.h"
#include "physics/include/aabb.h"
#include "physics/include/obb.h"
#include "physics/include/sphere_bound.h"


namespace physics
{
	AABB::AABB(const glm::vec3& min, const glm::vec3& max)
	{
		init(min, max);
	}

	void AABB::init(const glm::vec3& min, const glm::vec3& max)
	{
		_min = glm::min(min, max);
		_max = glm::max(min, max);
		_center = (_min + _max) / 2.0f;
		_halfExtent = (_max - _min) / 2.0f;
	}

	std::unique_ptr<BoundTypeBase> AABB::clone() const
	{
		return std::make_unique<AABB>(*this);
	}

	glType::BoundType AABB::get_bound_type() const
	{
		return glType::BoundType::AABB;
	}
	

	TouchingData AABB::is_touching(const AABB& other) const
	{
		return BoundTypeBase::is_touching(*this, other);
	}


	TouchingData AABB::is_touching(const OBB& other) const
	{
		return BoundTypeBase::is_touching(*this, other);
	}

	TouchingData AABB::is_touching(const SphereBound& other) const
	{
		return BoundTypeBase::sphere_check(other, *this);
	}

	glm::vec3 AABB::get_half_extent() const
	{
		return _halfExtent;
	}

	glm::vec3 AABB::get_min() const
	{
		return _min;
	}

	glm::vec3 AABB::get_max() const
	{
		return _max;
	}

	AABB::~AABB() = default;


	void AABB::move_reletive_to_dist(const glm::vec3& dist)
	{
		static glm::vec3 initalMin = _min;
		static glm::vec3 initalMax = _max;
		static glm::vec3 initalCenter = _center;

		_min = initalMin + dist;
		_max = initalMax + dist;
		_center = initalCenter + dist;

	} 


	void AABB::move(const glm::vec3& volocity, float deltaTime)
	{
		glm::vec3 dist = volocity * deltaTime;
		_min += dist;
		_max += dist;
		_center += dist;
	}

	std::array<glm::vec3, 8> physics::AABB::get_corners() const
	{
		return {
			glm::vec3(_min.x, _min.y, _min.z),
			glm::vec3(_max.x, _min.y, _min.z),
			glm::vec3(_max.x, _max.y, _min.z),
			glm::vec3(_min.x, _max.y, _min.z),
			glm::vec3(_min.x, _min.y, _max.z),
			glm::vec3(_max.x, _min.y, _max.z),
			glm::vec3(_max.x, _max.y, _max.z),
			glm::vec3(_min.x, _max.y, _max.z)
		};
	}

	void AABB::move(const glm::vec3& volocityTimesDeltaTime)
	{
		_min += volocityTimesDeltaTime;
		_max += volocityTimesDeltaTime;
		_center += volocityTimesDeltaTime;
	}

}

