#include "headers.h"
#include "physics/include/obb.h"

namespace physics
{

	OBB::OBB() = default;


	OBB::OBB(const glm::vec3& min, const glm::vec3& max, const glm::vec3& xyzRotation, float rotationDegree)
	{
		init(min, max, xyzRotation, rotationDegree);
	}


	void OBB::init(const glm::vec3& min, const glm::vec3& max, const glm::vec3& xyzRotation, float rotationDegree)
	{
		AABB::init(min, max);
		_xyzRotation = xyzRotation;
		_rotationDeg = std::clamp(rotationDegree, -89.9f, 89.9f);

		_rotationMat = glm::translate(glm::mat4(1.0f), _center);
		_rotationMat = glm::rotate(_rotationMat, glm::radians(_rotationDeg), _xyzRotation);
		_rotationMat = glm::translate(_rotationMat, -_center);

		_localMin = _min;
		_localMax = _max;

		_min = glm::vec3(_rotationMat * glm::vec4(_min, 1.0f));
		_max = glm::vec3(_rotationMat * glm::vec4(_max, 1.0f));


		_halfExtent = (_max - _min) * 0.5f;
	}


	glm::vec3 OBB::get_rotation() const
	{
		return _xyzRotation;
	}


	float OBB::get_rotation_degree() const
	{
		return _rotationDeg;
	}


	glm::mat4 OBB::get_rotation_matrix() const
	{
		return _rotationMat;
	}

	glm::vec3 OBB::get_local_min() const
	{
		return _localMin;
	}

	glm::vec3 OBB::get_local_max() const
	{
		return _localMax;
	}

	glm::vec3 OBB::get_aabb_wrap_min() const
	{
		return _center - get_extent();
	}

	glm::vec3 OBB::get_aabb_wrap_max() const
	{
		return _center + get_extent();

	}

	void OBB::move_reletive_to_dist(const glm::vec3& dist)
	{
		_center += dist;
		_rotationMat = glm::translate(_rotationMat, dist);
		_min += dist;
		_max += dist;
		// Update corners after moving
		auto corners = get_corners();
		for (auto& corner : corners)
		{
			corner += dist;
		}
	}

	void OBB::change(const glm::vec3& offset)
	{
		_center += offset;
		_min += offset;
		_max += offset;

		_rotationMat = glm::translate(glm::mat4(1.0f), _center);
		_rotationMat = glm::rotate(_rotationMat, glm::radians(_rotationDeg), _xyzRotation);
		_rotationMat = glm::translate(_rotationMat, -_center);

		_halfExtent = (_max - _min) * 0.5f;
	}

	void OBB::change_x(float offset)
	{
		_center.x += offset;
		_min.x += offset;
		_max.x += offset;
		_rotationMat = glm::translate(glm::mat4(1.0f), _center);
		_rotationMat = glm::rotate(_rotationMat, glm::radians(_rotationDeg), _xyzRotation);
		_rotationMat = glm::translate(_rotationMat, -_center);
		_halfExtent = (_max - _min) * 0.5f;
	}

	void OBB::change_y(float offset)
	{
		_center.y += offset;
		_min.y += offset;
		_max.y += offset;
		_rotationMat = glm::translate(glm::mat4(1.0f), _center);
		_rotationMat = glm::rotate(_rotationMat, glm::radians(_rotationDeg), _xyzRotation);
		_rotationMat = glm::translate(_rotationMat, -_center);
		_halfExtent = (_max - _min) * 0.5f;
	}

	void OBB::change_z(float offset)
	{
		_center.z += offset;
		_min.z += offset;
		_max.z += offset;
		_rotationMat = glm::translate(glm::mat4(1.0f), _center);
		_rotationMat = glm::rotate(_rotationMat, glm::radians(_rotationDeg), _xyzRotation);
		_rotationMat = glm::translate(_rotationMat, -_center);
		_halfExtent = (_max - _min) * 0.5f;
	}

	std::array<glm::vec3, 8> OBB::get_corners() const
	{
		glm::vec3 xAxisRotation = glm::vec3(_rotationMat[0]);
		glm::vec3 yAxisRotation = glm::vec3(_rotationMat[1]);
		glm::vec3 zAxisRotation = glm::vec3(_rotationMat[2]);
		return {
			_center + _halfExtent.x * xAxisRotation + _halfExtent.y * yAxisRotation + _halfExtent.z * zAxisRotation,
			_center - _halfExtent.x * xAxisRotation + _halfExtent.y * yAxisRotation + _halfExtent.z * zAxisRotation,
			_center + _halfExtent.x * xAxisRotation - _halfExtent.y * yAxisRotation + _halfExtent.z * zAxisRotation,
			_center - _halfExtent.x * xAxisRotation - _halfExtent.y * yAxisRotation + _halfExtent.z * zAxisRotation,
			_center + _halfExtent.x * xAxisRotation + _halfExtent.y * yAxisRotation - _halfExtent.z * zAxisRotation,
			_center - _halfExtent.x * xAxisRotation + _halfExtent.y * yAxisRotation - _halfExtent.z * zAxisRotation,
			_center + _halfExtent.x * xAxisRotation - _halfExtent.y * yAxisRotation - _halfExtent.z * zAxisRotation,
			_center - _halfExtent.x * xAxisRotation - _halfExtent.y * yAxisRotation - _halfExtent.z * zAxisRotation,
		};
	}

	OBB::~OBB() = default;


	glm::vec3 OBB::get_extent() const
	{
		return glm::abs(_rotationMat[0]) * _halfExtent.x +
			glm::abs(_rotationMat[1]) * _halfExtent.y +
			glm::abs(_rotationMat[2]) * _halfExtent.z;
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


}
/*
		_corners =
		{
	
		};
*/

