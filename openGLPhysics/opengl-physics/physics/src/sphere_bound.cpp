#include "headers.h"
#include "physics/include/sphere_bound.h"

namespace physics
{

	SphereBound::SphereBound(const glm::vec3& center, float radius)
	{
		init(center, radius);
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
	}

	void SphereBound::change(const glm::vec3& offset)
	{
		_center += offset;
	}

	void SphereBound::change_x(float offset)
	{
		_center.x += offset;
	}

	void SphereBound::change_y(float offset)
	{
		_center.y += offset;
	}

	void SphereBound::change_z(float offset)
	{
		_center.z += offset;
	}

	bool SphereBound::is_touching(const SphereBound& other) const
	{
		return BoundTypeBase::sphere_check(other, *this);
	}

	bool SphereBound::is_touching(const AABB& other) const
	{
		return BoundTypeBase::sphere_check(*this, other);
	}

	bool SphereBound::is_touching(const OBB& other) const
	{
		return BoundTypeBase::sphere_check(*this, other);
	}

}