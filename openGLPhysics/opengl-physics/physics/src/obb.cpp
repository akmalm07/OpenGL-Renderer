#include "headers.h"
#include "physics/include/obb.h"

namespace physics
{

	_OBB_COMMON_ANCESTOR::_OBB_COMMON_ANCESTOR() = default;


	_OBB_COMMON_ANCESTOR::_OBB_COMMON_ANCESTOR(const glm::vec3& min, const glm::vec3& max, const glm::vec3& xyzRotation, float rotationDegree)
	{
		init(min, max, xyzRotation, rotationDegree);
	}


	void _OBB_COMMON_ANCESTOR::init(const glm::vec3& min, const glm::vec3& max, const glm::vec3& xyzRotation, float rotationDegree)
	{
		_AABB_COMMON_ANCESTOR::init(min, max);
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


	glm::vec3 _OBB_COMMON_ANCESTOR::get_rotation() const
	{
		return _xyzRotation;
	}


	float _OBB_COMMON_ANCESTOR::get_rotation_degree() const
	{
		return _rotationDeg;
	}


	glm::mat4 _OBB_COMMON_ANCESTOR::get_rotation_matrix() const
	{
		return _rotationMat;
	}

	glm::vec3 _OBB_COMMON_ANCESTOR::get_local_min() const
	{
		return _localMin;
	}

	glm::vec3 _OBB_COMMON_ANCESTOR::get_local_max() const
	{
		return _localMax;
	}

	glm::vec3 _OBB_COMMON_ANCESTOR::get_aabb_wrap_min() const
	{
		return _center - get_extent();
	}

	glm::vec3 _OBB_COMMON_ANCESTOR::get_aabb_wrap_max() const
	{
		return _center + get_extent();

	}

	_OBB_COMMON_ANCESTOR::~_OBB_COMMON_ANCESTOR() = default;


	glm::vec3 _OBB_COMMON_ANCESTOR::get_extent() const
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


	_OBB<_PRECALC_COORDS::NO>::_OBB() = default;

	_OBB<_PRECALC_COORDS::NO>::_OBB(const glm::vec3& min, const glm::vec3& max, const glm::vec3& xyzRotation, float rotationDegree)
	{
		_OBB_COMMON_ANCESTOR::init(min, max, xyzRotation, rotationDegree);
	}

	_OBB<_PRECALC_COORDS::NO>::~_OBB() = default;


	_OBB<_PRECALC_COORDS::YES>::_OBB() = default;

	_OBB<_PRECALC_COORDS::YES>::_OBB(const glm::vec3& min, const glm::vec3& max, const glm::vec3& xyzRotation, float rotationDegree)
	{
		_OBB_COMMON_ANCESTOR::init(min, max, xyzRotation, rotationDegree);

		glm::vec3 xAxisRotation = glm::vec3(_rotationMat[0]);
		glm::vec3 yAxisRotation = glm::vec3(_rotationMat[1]);
		glm::vec3 zAxisRotation = glm::vec3(_rotationMat[2]);

		_corners =
		{
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

	_OBB<_PRECALC_COORDS::YES>::~_OBB() = default;

}

