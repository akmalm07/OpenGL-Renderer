#include "headers.h"
#include "physics/include/sphere_bound.h"

namespace physics
{

	SphereBound::SphereBound(const glm::vec3& center, float radius)
	{
		init(center, radius);
	}

	SphereBound::SphereBound(const glm::vec3& min, const glm::vec3& max, float radius)
	{
		glm::vec3 center = (min + max) * 0.5f;
		init(center, radius);
	}

	std::unique_ptr<BoundTypeBase> SphereBound::clone() const
	{
		return std::make_unique<SphereBound>(*this);
	}

	glType::BoundType SphereBound::get_bound_type() const
	{
		return glType::BoundType::Sphere;
	}

	void SphereBound::init(const glm::vec3& center, float radius)
	{
		_min = center - glm::vec3(radius);
		_max = center + glm::vec3(radius);

		_center = center;
		_radius = radius;
	}

	float SphereBound::get_radius() const
	{
		return _radius;
	}

	void SphereBound::move_reletive_to_dist(const glm::vec3& dist)
	{
		_center += dist;
		_min += dist;
		_max += dist;
	}

	TouchingData SphereBound::is_touching(const SphereBound& other) const
	{
		return BoundTypeBase::sphere_check(other, *this);
	}

	TouchingData SphereBound::is_touching(const AABB& other) const
	{
		return BoundTypeBase::sphere_check(*this, other);
	}

	TouchingData SphereBound::is_touching(const OBB& other) const
	{
		return BoundTypeBase::sphere_check(*this, other);
	}

}