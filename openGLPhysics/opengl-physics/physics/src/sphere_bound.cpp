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
		_center = center;
		_radius = radius;
	}

	float SphereBound::get_radius() const
	{
		return _radius;
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