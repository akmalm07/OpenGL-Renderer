#include "headers.h"
#include "physics/include/aabb.h"


namespace physics
{
	_AABB_COMMON_ANCESTOR::_AABB_COMMON_ANCESTOR() = default;

	_AABB_COMMON_ANCESTOR::_AABB_COMMON_ANCESTOR(const glm::vec3& min, const glm::vec3& max)
	{
		init(min, max);
	}

	void _AABB_COMMON_ANCESTOR::init(const glm::vec3& min, const glm::vec3& max)
	{
		_min = glm::min(min, max);
		_max = glm::max(min, max);
		_center = (_min + _max) / 2.0f;
		_halfExtent = (_max - _min) / 2.0f;
	}

	glm::vec3 _AABB_COMMON_ANCESTOR::get_center() const
	{
		return _center;
	}

	glm::vec3 _AABB_COMMON_ANCESTOR::get_min() const
	{
		return _min;
	}

	glm::vec3 _AABB_COMMON_ANCESTOR::get_max() const
	{
		return _max;
	}

	MinMax _AABB_COMMON_ANCESTOR::get_min_max() const
	{
		return { _min, _max };
	}

	_AABB_COMMON_ANCESTOR::~_AABB_COMMON_ANCESTOR() = default;


	glm::vec3 _AABB_COMMON_ANCESTOR::get_half_extent() const
	{
		return _halfExtent;
	}

	void _AABB_COMMON_ANCESTOR::change(const glm::vec3& val)
	{
		_min += val;
		_max += val;
		_center += val;
	}

	void _AABB_COMMON_ANCESTOR::move_x_reletive_to_dist(float dist)
	{

	}

	void _AABB_COMMON_ANCESTOR::move_y_reletive_to_dist(float dist)
	{
	}

	void _AABB_COMMON_ANCESTOR::move_z_reletive_to_dist(float dist)
	{
	}

	void _AABB_COMMON_ANCESTOR::move_reletive_to_dist(const glm::vec3& dist)
	{
		static glm::vec3 initalMin = _min;
		static glm::vec3 initalMax = _max;
		static glm::vec3 initalCenter = _center;

		_min = initalMin + dist;
		_max = initalMax + dist;
		_center = initalCenter + dist;

	} 

	void _AABB_COMMON_ANCESTOR::change_x(float val)
	{
		_min.x += val;
		_max.x += val;
		_center.x += val;
	}

	void _AABB_COMMON_ANCESTOR::change_y(float val)
	{
		_min.y += val;
		_max.y += val;
		_center.y += val;
	}

	void _AABB_COMMON_ANCESTOR::change_z(float val)
	{
		_min.z += val;
		_max.z += val;
		_center.z += val;
	}

	void _AABB_COMMON_ANCESTOR::move(const glm::vec3& volocity, float deltaTime)
	{
		glm::vec3 dist = volocity * deltaTime;
		_min += dist;
		_max += dist;
		_center += dist;
	}

	void _AABB_COMMON_ANCESTOR::move(const glm::vec3& volocityTimesDeltaTime)
	{
		_min += volocityTimesDeltaTime;
		_max += volocityTimesDeltaTime;
		_center += volocityTimesDeltaTime;
	}


	_AABB<_PRECALC_COORDS::NO> ::_AABB() = default;

	_AABB<_PRECALC_COORDS::NO> ::_AABB(const glm::vec3& min, const glm::vec3& max)
		: _AABB_COMMON_ANCESTOR(min, max)
	{}

	_AABB<_PRECALC_COORDS::NO>::~_AABB() = default;


	_AABB<_PRECALC_COORDS::YES> ::_AABB() = default;

	_AABB<_PRECALC_COORDS::YES> ::_AABB(const glm::vec3& min, const glm::vec3& max)
		: _AABB_COMMON_ANCESTOR(min, max)
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
		_AABB_COMMON_ANCESTOR::move_reletive_to_dist(dist);

		std::array<glm::vec3, 8> initalCorners = _corners;

		for (int i = 0; i < 8; i++)
		{
			_corners[i] += initalCorners[i] + dist;
		}
	}

	void _AABB<_PRECALC_COORDS::YES>::move_x_reletive_to_dist(float dist)
	{
		_AABB_COMMON_ANCESTOR::move_x_reletive_to_dist(dist);
		std::array<glm::vec3, 8> initalCorners = _corners;

		for (int i = 0; i < 8; i++)
		{
			_corners[i].x += initalCorners[i].x + dist;
		}
	}

	void _AABB<_PRECALC_COORDS::YES>::move_y_reletive_to_dist(float dist)
	{
		_AABB_COMMON_ANCESTOR::move_y_reletive_to_dist(dist);
		std::array<glm::vec3, 8> initalCorners = _corners;

		for (int i = 0; i < 8; i++)
		{
			_corners[i].x += initalCorners[i].y + dist;
		}
	}

	void _AABB<_PRECALC_COORDS::YES>::move_z_reletive_to_dist(float dist)
	{
		_AABB_COMMON_ANCESTOR::move_z_reletive_to_dist(dist);
		std::array<glm::vec3, 8> initalCorners = _corners;

		for (int i = 0; i < 8; i++)
		{
			_corners[i].x += initalCorners[i].z + dist;
		}
	}

	void _AABB<_PRECALC_COORDS::YES>::move(const glm::vec3& volocity, float deltaTime)
	{
		glm::vec3 dist = volocity * deltaTime;
		_AABB_COMMON_ANCESTOR::move(dist);
		for (int i = 0; i < 8; i++)
		{
			_corners[i] += dist;
		}
	}

	void _AABB<_PRECALC_COORDS::YES>::move(const glm::vec3& volocityTimesDeltaTime)
	{
		_AABB_COMMON_ANCESTOR::move(volocityTimesDeltaTime);
		for (int i = 0; i < 8; i++)
		{
			_corners[i] += volocityTimesDeltaTime;
		}
	}

	void _AABB<_PRECALC_COORDS::YES>::change(const glm::vec3& offset)
	{
		_AABB_COMMON_ANCESTOR::change(offset);
		for (int i = 0; i < 8; i++)
		{
			_corners[i] += offset;
		}
	}

	void _AABB<_PRECALC_COORDS::YES>::change_x(float offset)
	{
		_AABB_COMMON_ANCESTOR::change_x(offset);
		for (int i = 0; i < 8; i++)
		{
			_corners[i].x += offset;
		}
	}

	void _AABB<_PRECALC_COORDS::YES>::change_y(float offset)
	{
		_AABB_COMMON_ANCESTOR::change_y(offset);
		for (int i = 0; i < 8; i++)
		{
			_corners[i].y += offset;
		}
	}

	void _AABB<_PRECALC_COORDS::YES>::change_z(float offset)
	{
		_AABB_COMMON_ANCESTOR::change_z(offset);
		for (int i = 0; i < 8; i++)
		{
			_corners[i].z += offset;
		}
	}

	_AABB<_PRECALC_COORDS::YES>::~_AABB() = default;

	MinMax get_min_max_from_vertices(const std::vector<glType::Vertex>& verts, FullStride fullStride, PosStride posStride)
	{

		if (verts.empty())
		{
			return { glm::vec3(0.0f), glm::vec3(0.0f) };
		}

		std::vector<glm::vec3> vertices;

		for (size_t i = 0; i < verts.size(); i += static_cast<size_t>(fullStride))
		{

			glm::vec3 vertex;

			vertex.x = verts[i];
			vertex.y = verts[i + 1];

			if (posStride == PosStride::STRIDE_3D)
			{
				vertex.z = verts[i + 2];
			}
			else if (posStride == PosStride::STRIDE_2D)
			{
				vertex.z = 0.0f;
			}

			vertices.push_back(vertex);
		}

		glm::vec3 minVec = vertices[0];
		glm::vec3 maxVec = vertices[0];

		for (const auto& vertex : vertices)
		{
			minVec = glm::min(minVec, vertex);
			maxVec = glm::max(maxVec, vertex);
		}

		return { minVec, maxVec };
	}

}

