#pragma once

#include "physics/include/aabb.h"
#include "physics/include/obb.h"

namespace physics
{

	bool AABB::is_touching(const AABB& other) const
	{
		return _CHECK_COLLISION::is_touching(*this, other);
	}

	
	bool AABB::is_touching(const OBB& other) const
	{
		return _CHECK_COLLISION::is_touching(*this, other);
	}

} // namespace physics



/*


bool _AABB <_PRECALC_COORDS::YES>::is_touching(const AABB& other) const
{
	return _CHECK_COLLISION::is_touching(*this, other);
}


bool _AABB <_PRECALC_COORDS::YES>::is_touching(const OBB& other) const
{
	return _CHECK_COLLISION::is_touching(*this, other);
}

*/

/*
_AABB<_PRECALC_COORDS::YES> ::_AABB() = default;

_AABB<_PRECALC_COORDS::YES> ::_AABB(const glm::vec3& min, const glm::vec3& max)
	: AABB(min, max)
{
	_corners =
	{
		min,
		glm::vec3(max.x, min.y, min.z),
		glm::vec3(min.x, max.y, min.z),
		glm::vec3(max.x, max.y, min.z),
		glm::vec3(min.x, min.y, max.z),
		glm::vec3(max.x, min.y, max.z),
		glm::vec3(min.x, max.y, max.z),
		max
	};
}

std::array<glm::vec3, 8> _AABB<_PRECALC_COORDS::YES>::get_corners() const
{
	return _corners;
}

std::array<glType::Vertex, 48> _AABB<_PRECALC_COORDS::YES>::get_vertices(const glm::vec3& color) const
{
	return {
	_corners[0].x, _corners[0].y, _corners[0].z, color.x, color.y, color.z,
	_corners[1].x, _corners[1].y, _corners[1].z, color.x, color.y, color.z,
	_corners[2].x, _corners[2].y, _corners[2].z, color.x, color.y, color.z,
	_corners[3].x, _corners[3].y, _corners[3].z, color.x, color.y, color.z,
	_corners[4].x, _corners[4].y, _corners[4].z, color.x, color.y, color.z,
	_corners[5].x, _corners[5].y, _corners[5].z, color.x, color.y, color.z,
	_corners[6].x, _corners[6].y, _corners[6].z, color.x, color.y, color.z,
	_corners[7].x, _corners[7].y, _corners[7].z, color.x, color.y, color.z
	};
}

std::array<glType::Index, 36> _AABB<_PRECALC_COORDS::YES>::get_indices() const
{
	return
	{
   0, 2, 1,
	1, 2, 3,

	// Back face (z = max.z) - Counter-clockwise
	4, 6, 5,
	5, 6, 7,

	// Left face (x = min.x) - Counter-clockwise
	0, 4, 2,
	2, 4, 6,

	// Right face (x = max.x) - Counter-clockwise
	1, 3, 5,
	5, 3, 7,

	// Top face (y = max.y) - Counter-clockwise
	2, 6, 3,
	3, 6, 7,

	// Bottom face (y = min.y) - Counter-clockwise
	0, 1, 4,
	4, 1, 5

	};
}

void _AABB<_PRECALC_COORDS::YES>::move_reletive_to_dist(const glm::vec3& dist)
{
	AABB::move_reletive_to_dist(dist);

	std::array<glm::vec3, 8> initalCorners = _corners;

	for (int i = 0; i < 8; i++)
	{
		_corners[i] += initalCorners[i] + dist;
	}
}

void _AABB<_PRECALC_COORDS::YES>::move_x_reletive_to_dist(float dist)
{
	AABB::move_x_reletive_to_dist(dist);
	std::array<glm::vec3, 8> initalCorners = _corners;

	for (int i = 0; i < 8; i++)
	{
		_corners[i].x += initalCorners[i].x + dist;
	}
}

void _AABB<_PRECALC_COORDS::YES>::move_y_reletive_to_dist(float dist)
{
	AABB::move_y_reletive_to_dist(dist);
	std::array<glm::vec3, 8> initalCorners = _corners;

	for (int i = 0; i < 8; i++)
	{
		_corners[i].x += initalCorners[i].y + dist;
	}
}

void _AABB<_PRECALC_COORDS::YES>::move_z_reletive_to_dist(float dist)
{
	AABB::move_z_reletive_to_dist(dist);
	std::array<glm::vec3, 8> initalCorners = _corners;

	for (int i = 0; i < 8; i++)
	{
		_corners[i].x += initalCorners[i].z + dist;
	}
}

void _AABB<_PRECALC_COORDS::YES>::move(const glm::vec3& volocity, float deltaTime)
{
	glm::vec3 dist = volocity * deltaTime;
	AABB::move(dist);
	for (int i = 0; i < 8; i++)
	{
		_corners[i] += dist;
	}
}

void _AABB<_PRECALC_COORDS::YES>::move(const glm::vec3& volocityTimesDeltaTime)
{
	AABB::move(volocityTimesDeltaTime);
	for (int i = 0; i < 8; i++)
	{
		_corners[i] += volocityTimesDeltaTime;
	}
}

void _AABB<_PRECALC_COORDS::YES>::change(const glm::vec3& offset)
{
	AABB::change(offset);
	for (int i = 0; i < 8; i++)
	{
		_corners[i] += offset;
	}
}

void _AABB<_PRECALC_COORDS::YES>::change_x(float offset)
{
	AABB::change_x(offset);
	for (int i = 0; i < 8; i++)
	{
		_corners[i].x += offset;
	}
}

void _AABB<_PRECALC_COORDS::YES>::change_y(float offset)
{
	AABB::change_y(offset);
	for (int i = 0; i < 8; i++)
	{
		_corners[i].y += offset;
	}
}

void _AABB<_PRECALC_COORDS::YES>::change_z(float offset)
{
	AABB::change_z(offset);
	for (int i = 0; i < 8; i++)
	{
		_corners[i].z += offset;
	}
}

_AABB<_PRECALC_COORDS::YES>::~_AABB() = default;
*/