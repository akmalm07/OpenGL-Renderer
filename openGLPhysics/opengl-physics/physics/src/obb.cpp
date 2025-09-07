#include "headers.h"
#include "physics/include/obb.h"

namespace physics
{

	OBB::OBB() = default;

	OBB::OBB(const glm::vec3& min, const glm::vec3& max, const glm::vec3& degrees)
	{
		_center = (min + max) * 0.5f;
		_halfExtent = (max - min) * 0.5f;

		rotate(degrees);
	}

	OBB::OBB(const glm::vec3& center, float length, float width, float height, const glm::vec3& degrees)
	{
		_center = center;
		_halfExtent = glm::vec3(length, width, height) * 0.5f;

		rotate(degrees);

	}

	std::unique_ptr<BoundTypeBase> OBB::clone() const
	{
		return std::make_unique<OBB>(*this);
	}

	glType::BoundType OBB::get_bound_type() const
	{
		return glType::BoundType::OBB;
	}

	glm::mat3 OBB::get_rotation() const 
	{ 
		return glm::mat3(_orientation); 
	}


	MinMax OBB::get_aabb_wrap() const
	{
		return { _center - _halfExtent, _center + _halfExtent };
	}

	float OBB::get_volume() const
	{
		return 8.0f * _halfExtent.x * _halfExtent.y * _halfExtent.z;
	}

	MinMax OBB::get_min_max() const
	{
		return { _orientation * (_center + _halfExtent), _orientation * (_center - _halfExtent) };
	}


	void OBB::rotate(const glm::vec3& deg)
	{
		glm::quat qx = glm::angleAxis(glm::radians(deg.x), glm::vec3(1, 0, 0));
		glm::quat qy = glm::angleAxis(glm::radians(deg.y), glm::vec3(0, 1, 0));
		glm::quat qz = glm::angleAxis(glm::radians(deg.z), glm::vec3(0, 0, 1));

		glm::quat q = qz * qy * qx; 
		_orientation = glm::normalize(q * _orientation);
	}



	MinMax OBB::get_aabb_wrap_min_max() const
	{
		return { _center + _halfExtent, _center - _halfExtent };
	}


	CollisionPoint OBB::touching(const AABB& other) const
	{
		return CollisionChecker::partial_sat_check(*this, other);
	}

	CollisionPoint OBB::touching(const OBB& other) const
	{
		return CollisionChecker::full_sat_check(*this, other);
	}

	CollisionPoint OBB::touching(const SphereBound& other) const
	{
		return CollisionChecker::sphere_check(other, *this);
	}

	void OBB::move(const glm::vec3& offset)
	{
		_center += offset;
	}

	void OBB::full_move(const glm::vec3& offset, const glm::vec3& rotation)
	{
		move(offset);
		rotate(rotation);
	}

	void OBB::set_pos(const glm::vec3& pos)
	{
		_center = pos;
	}


	OBB::~OBB() = default;


	glm::vec3 projection(const glm::vec3& v, const glm::vec3& u)
	{
		return (glm::dot(v, u) / glm::dot(u, u)) * u;
	}

	glm::vec3 project_onto_plane(const glm::vec3& vec, const glm::vec3& plane)
	{
		return vec - projection(vec, plane);
	}

	glm::vec3 project_onto_axis(const glm::vec3& vec, Axis axis)
	{
		switch (axis)
		{
		case Axis::X:
			return glm::vec3(vec.x, 0.0f, 0.0f);
		case Axis::Y:
			return glm::vec3(0.0f, vec.y, 0.0f);
		case Axis::Z:
			return glm::vec3(0.0f, 0.0f, vec.z);
		default:
			return vec;
		}
	}

} // namespace physics
