#pragma once

#include "physics/include/spatial_partioning.h"
#include "physics/include/physics_body.h"


namespace physics
{
	template<size_t CellCount>
	SpatialPartioning<CellCount>::SpatialPartioning(const glm::vec3& minBound, const glm::vec3& maxBound)
		: _cellWidth((maxBound.x - minBound.x) / CellCount),
		_cellHeight((maxBound.y - minBound.y) / CellCount)
	{
		for (size_t i = 0; i < CellCount; ++i)
		{
			_cellMinBounds[i] = glm::vec3(minBound.x + i * _cellWidth, minBound.y, minBound.z);
			_cellMaxBounds[i] = glm::vec3(minBound.x + (i + 1) * _cellWidth, maxBound.y, maxBound.z);
		}
	}

	template<size_t CellCount>
	inline static void SpatialPartioning<CellCount>::register_body(const PhysicsBodyBase& body)
	{
		MinMax position = body.get_aabb();

		for (size_t i = 0; i < CellCount; i++)
		{
			if (position.min.x >= _cellMinBounds[i].x && position.max.x <= _cellMaxBounds[i].x &&
				position.min.y >= _cellMinBounds[i].y && position.max.y <= _cellMaxBounds[i].y)
			{
				_cells[i].push_back(body.get_entity_id());
			}
		}
	}


}