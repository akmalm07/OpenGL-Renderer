#include "headers.h"
#include "physics/include/sphere_bound.h"

namespace physics
{

	SphereBound::SphereBound(const glm::vec3& center, float radius)
	{
		_center = center;
		_radius = radius;
	}

	SphereBound::SphereBound(const glm::vec3& min, const glm::vec3& max, float radius)
	{
		_center = (min + max) * 0.5f;
		_radius = radius;
	}

	std::unique_ptr<BoundTypeBase> SphereBound::clone() const
	{
		return std::make_unique<SphereBound>(*this);
	}

	MinMax SphereBound::get_aabb_wrap() const
	{
		return { _center - glm::vec3(_radius), _center + glm::vec3(_radius) };
	}

	float SphereBound::get_volume() const
	{
		return (4.0f / 3.0f) * glm::pi<float>() * _radius * _radius * _radius;
	}

	glType::BoundType SphereBound::get_bound_type() const
	{
		return glType::BoundType::Sphere;
	}

	float SphereBound::get_radius() const
	{
		return _radius;
	}



	glm::vec3 SphereBound::get_center() const
	{
		return _center;
	}

	void SphereBound::move(const glm::vec3& offset)
	{
		_center += offset;
	}

	CollisionPoint SphereBound::touching(const SphereBound& other) const
	{
		return CollisionChecker::sphere_check(other, *this);
	}

	CollisionPoint SphereBound::touching(const AABB& other) const
	{
		return CollisionChecker::sphere_check(*this, other);
	}

	CollisionPoint SphereBound::touching(const OBB& other) const
	{
		return CollisionChecker::sphere_check(*this, other);
	}

}