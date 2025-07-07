#include "headers.h"
#include "physics/include/obb.h"

namespace physics
{

	OBB::OBB() = default;

	OBB::OBB(const glm::vec3& min, const glm::vec3& max, const glm::vec3& xyzRotation)
	{
		init(min, max, xyzRotation);
	}

	void OBB::init(const glm::vec3& min, const glm::vec3& max, const glm::vec3& rotationDegrees)
	{
		_min = min;
		_max = max;

		_center = (_min + _max) * 0.5f;
		_halfExtent = (_max - _min) * 0.5f;

		glm::mat4 rotMat(1.0f);
		rotMat = glm::rotate(rotMat, glm::radians(rotationDegrees.z), glm::vec3(0, 0, 1));
		rotMat = glm::rotate(rotMat, glm::radians(rotationDegrees.y), glm::vec3(0, 1, 0));
		rotMat = glm::rotate(rotMat, glm::radians(rotationDegrees.x), glm::vec3(1, 0, 0));
		
		_rotationMat = rotMat;

		_rotation = rotationDegrees;
	}

	std::unique_ptr<BoundTypeBase> OBB::clone() const
	{
		return std::make_unique<OBB>(*this);
	}

	glType::BoundType OBB::get_bound_type() const
	{
		return glType::BoundType::OBB;
	}

	glm::vec3 OBB::get_rotation() const 
	{ 
		return _xyzRotation; 
	}

	glm::vec3 OBB::get_rotation_degree() const 
	{ 
		return _rotation; 
	}

	glm::mat4 OBB::get_rotation_matrix() const 
	{ 
		return _rotationMat; 
	}

	MinMax OBB::get_rotated_min_max() const
	{
		glm::vec3 extent = get_extent();
		return { _center - extent, _center + extent };
	}


	MinMax OBB::get_aabb_wrap_min_max() const
	{
		return {_center - get_extent(), _center + get_extent() };
	}


	TouchingData OBB::is_touching(const AABB& other) const
	{
		return BoundTypeBase::is_touching(other, *this);
	}

	TouchingData OBB::is_touching(const OBB& other) const
	{
		return BoundTypeBase::is_touching(*this, other);
	}

	TouchingData OBB::is_touching(const SphereBound& other) const
	{
		return BoundTypeBase::sphere_check(other, *this);
	}


	void OBB::change(const glm::vec3& offset, const glm::vec3& rotation)
	{
		_center += offset;
		_max += offset;
		_min += offset;

		_xyzRotation = rotation;
		_rotationMat = glm::translate(glm::mat4(1.0f), _center);
		_rotationMat = glm::rotate(_rotationMat, glm::radians(rotation.x), glm::vec3(1, 0, 0));
		_rotationMat = glm::rotate(_rotationMat, glm::radians(rotation.y), glm::vec3(0, 1, 0));
		_rotationMat = glm::rotate(_rotationMat, glm::radians(rotation.z), glm::vec3(0, 0, 1));
		_rotationMat = glm::translate(_rotationMat, -_center);

		_min = glm::vec3(_rotationMat * glm::vec4(_min, 1.0f));
		_max = glm::vec3(_rotationMat * glm::vec4(_max, 1.0f));

		_halfExtent = (_max - _min) * 0.5f;
	}

	void OBB::change_x(float offset, float rotation)
	{
		_center.x += offset;
		_min.x += offset;
		_max.x += offset;
		_xyzRotation.x += rotation;
		_rotationMat = glm::rotate(_rotationMat, glm::radians(rotation), glm::vec3(1, 0, 0));

		_min = glm::vec3(_rotationMat * glm::vec4(_min, 1.0f));
		_max = glm::vec3(_rotationMat * glm::vec4(_max, 1.0f));
	
		_halfExtent = (_max - _min) * 0.5f;
	}

	void OBB::change_y(float offset, float rotation)
	{
		_center.y += offset;
		_min.y += offset;
		_max.y += offset;
		_xyzRotation.y += rotation;
		_rotationMat = glm::translate(glm::mat4(1.0f), _center);
		_rotationMat = glm::rotate(_rotationMat, glm::radians(rotation), glm::vec3(0, 1, 0));
		_rotationMat = glm::translate(_rotationMat, -_center);

		_min = glm::vec3(_rotationMat * glm::vec4(_min, 1.0f));
		_max = glm::vec3(_rotationMat * glm::vec4(_max, 1.0f));

		_halfExtent = (_max - _min) * 0.5f;
	}

	void OBB::change_z(float offset, float rotation)
	{
		_center.z += offset;
		_min.z += offset;
		_max.z += offset;

		_xyzRotation.z += rotation;
		_rotationMat = glm::translate(glm::mat4(1.0f), _center);
		_rotationMat = glm::rotate(_rotationMat, glm::radians(rotation), glm::vec3(0, 0, 1));
		_rotationMat = glm::translate(_rotationMat, -_center);

		_min = glm::vec3(_rotationMat * glm::vec4(_min, 1.0f));
		_max = glm::vec3(_rotationMat * glm::vec4(_max, 1.0f));

		_halfExtent = (_max - _min) * 0.5f;
	}

	void OBB::move(const glm::vec3& offset)
	{
		_center += offset;
		_min += offset;
		_max += offset;
	}

	void OBB::rotate(const glm::vec3& rotation)
	{
		_xyzRotation += rotation;

		_rotationMat = glm::rotate(_rotationMat, glm::radians(rotation.x), glm::vec3(1, 0, 0));
		_rotationMat = glm::rotate(_rotationMat, glm::radians(rotation.y), glm::vec3(0, 1, 0));
		_rotationMat = glm::rotate(_rotationMat, glm::radians(rotation.z), glm::vec3(0, 0, 1));
		
		_min = glm::vec3(_rotationMat * glm::vec4(_min, 1.0f));
		_max = glm::vec3(_rotationMat * glm::vec4(_max, 1.0f));

		_halfExtent = (_max - _min) * 0.5f;
	}

	std::array<glm::vec3, 8> OBB::get_corners() const
	{
		std::array<glm::vec3, 8> localCorners = {
			glm::vec3(_halfExtent.x,  _halfExtent.y,  _halfExtent.z),
			glm::vec3(-_halfExtent.x,  _halfExtent.y,  _halfExtent.z),
			glm::vec3(_halfExtent.x, -_halfExtent.y,  _halfExtent.z),
			glm::vec3(-_halfExtent.x, -_halfExtent.y,  _halfExtent.z),
			glm::vec3(_halfExtent.x,  _halfExtent.y, -_halfExtent.z),
			glm::vec3(-_halfExtent.x,  _halfExtent.y, -_halfExtent.z),
			glm::vec3(_halfExtent.x, -_halfExtent.y, -_halfExtent.z),
			glm::vec3(-_halfExtent.x, -_halfExtent.y, -_halfExtent.z),
		};

		std::array<glm::vec3, 8> worldCorners;
		for (int i = 0; i < 8; ++i)
		{
			worldCorners[i] = _center + _rotation * localCorners[i];
		}

		return worldCorners;
	}	

	OBB::~OBB() = default;

	glm::vec3 OBB::get_extent() const
	{
		return glm::abs(_rotationMat[0]) * _halfExtent.x +
			glm::abs(_rotationMat[1]) * _halfExtent.y +
			glm::abs(_rotationMat[2]) * _halfExtent.z;
	}

	void OBB::update_before_calc()
	{

	}

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
