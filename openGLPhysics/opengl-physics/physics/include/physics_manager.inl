#pragma once

#include "physics/include/physics_manager.h"

#include "physics/include/physics_body.h"

#include "tools/include/component_registry.h"

namespace physics
{
	template<size_t CellCount>
	PhysicsManager<CellCount>::PhysicsManager(const glm::vec3& minBound, const glm::vec3& maxBound)
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
	inline void PhysicsManager<CellCount>::register_body(const PhysicsBody& body)
	{
		MinMax position = body.get_aabb();
		
		//bool firstTime = true;
		
		for (size_t i = 0; i < CellCount; i++)
		{
			const glm::vec2& cellMin = _cellMinBounds[i];
			const glm::vec2& cellMax = _cellMaxBounds[i];

			bool intersects =
				position.max.x >= cellMin.x && position.min.x <= cellMax.x &&
				position.max.y >= cellMin.y && position.min.y <= cellMax.y;

			if (intersects)
			{
				_cells[i].push_back(body.get_entity_id());
				

				if (_participatingEntities.ents.empty() || _participatingEntities.ents.back() != body.get_entity_id())
				{
					_participatingEntities.vol.push_back(body.get_volume());

					_participatingEntities.ents.push_back(body.get_entity_id());

					_participatingEntities.index.push_back(i);
				}
			}
		}
	}

	template<size_t CellCount>
	inline void PhysicsManager<CellCount>::update()
	{

		update_positions();

		respond_to_collisions();
	}

	template<size_t CellCount>
	inline void PhysicsManager<CellCount>::update_positions()
	{
		const auto& physBodyComponentInstance = tools::ComponentRegistry<PhysicsBody>::get_instance();

		for (size_t i = 0; i < _participatingEntities.ents.size(); i++)
		{
			PhysicsBody* body = physBodyComponentInstance.get_component_or_null(_participatingEntities.ents[i]);

			if (!body || !body->has_force())
			{
				continue;
			}


			size_t initialVal = (_participatingEntities.index[i] == 0 ? 0 : _participatingEntities.index[i - 1]);
			float totalCollectedVol = 0.0f;

			//Forward Scan
			for (size_t j = initialVal; j < CellCount; j++)
			{
				totalCollectedVol += run_check_of_body_index(body, j, initialVal);
				if (totalCollectedVol >= _participatingEntities.vol[i])
				{
					break;
				}
			}

			if (totalCollectedVol >= _participatingEntities.vol[i])
			{
				continue;
			}

			// Backward Scan. Also checks for j == 0 to avoid underflow
			for (size_t j = initialVal; j-- > 0;)
			{
				totalCollectedVol += run_check_of_body_index(body, j, initialVal);
				if (totalCollectedVol >= _participatingEntities.vol[i])
				{
					break;
				}
			}
		}
	}

	template<size_t CellCount>
	inline void PhysicsManager<CellCount>::respond_to_collisions()
	{
		const auto& physBodyComponentInstance = tools::ComponentRegistry<PhysicsBody>::get_instance();

		for (size_t i = 0; i < _cells.size(); i++)
		{
			if (_cells[i].empty() || _cells[i].size() == 1)
			{
				continue;
			}

			for (size_t j = 0; j < _cells[i].size(); j++)
			{
				PhysicsBody* physBody = physBodyComponentInstance.get_component_or_null(_cells[i][j]);

				if (!physBody)
				{
					continue;
				}

				for (size_t k = j + 1; k < _cells[i].size(); k++) // O(n^2) time
				{
					PhysicsBody* other = physBodyComponentInstance.get_component_or_null(_cells[i][k]);
					if (!other)
					{
						continue;
					}
					if (physBody->is_colliding(*other))
					{
						collision_response(physBody, other);
					}
				}
			}
		}
	}


	template<size_t CellCount>
	inline void PhysicsManager<CellCount>::collision_response(PhysicsBody* body, PhysicsBody* other) const
	{

		//Natural Elastic Collision Response

		glm::vec3 v1 = body->get_volocity();
		glm::vec3 v2 = other->get_volocity();

		float mass1 = body->get_mass();
		float mass2 = other->get_mass();

		glm::vec3 normal = glm::normalize(other->get_position() - body->get_position()); // collision normal

		float volocity1norm = glm::dot(v1, normal);
		float volocity2norm = glm::dot(v2, normal);

		float volocity1normScale = (volocity1norm   * (mass1 - mass2) + 2 * mass2 * volocity2norm) / (mass1 + mass2);
		float volocity2normScale = (volocity2norm * (mass2 - mass1) + 2 * mass1 * volocity1norm  ) / (mass1 + mass2);

		glm::vec3 volocity1normVec = normal * volocity1normScale;
		glm::vec3 volocity2normVec = normal * volocity2normScale;

		glm::vec3 volocity1tan = v1 - normal * volocity1norm;
		glm::vec3 volocity2tan = v2 - normal * volocity2norm;

		body->set_volocity(volocity1tan + volocity1normVec);
		other->set_volocity(volocity2tan + volocity2normVec);


		//User defined collision response callback
		body->collision_response_callback(other->get_entity_id());
		other->collision_response_callback(body->get_entity_id());

	}



	template<size_t CellCount>
	inline float PhysicsManager<CellCount>::run_check_of_body_index(PhysicsBody* body, size_t index, size_t initalIndex)
	{
		const glm::vec2& cellMin = _cellMinBounds[index];
		const glm::vec2& cellMax = _cellMaxBounds[index];
		
		MinMax position = body->get_aabb();

		bool intersects =
			position.max.x >= cellMin.x && position.min.x <= cellMax.x &&
			position.max.y >= cellMin.y && position.min.y <= cellMax.y;

		auto it = std::find(_cells[index].begin(), _cells[index].end(), body->get_entity_id());
		
		if (it == _cells[index].end())
		{
			return 0.0f;
		}
		
		if (intersects)
		{
			if (it == _cells[index].end())
			{
				_cells[index].push_back(body->get_entity_id());
			}

			_participatingEntities.index[initalIndex] = index;

			return get_vol_of_entity_in_cell(body, index);
		}
		else
		{	
			_cells[index].erase(it);
			return 0.0f;
		}
		

	}

	template<size_t CellCount>
	inline float PhysicsManager<CellCount>::get_vol_of_entity_in_cell(PhysicsBody* body, size_t index) const
	{

		MinMax aabb = body->get_aabb();

		glm::vec3 cellMin = _cellMinBounds[index];
		glm::vec3 cellMax = _cellMaxBounds[index];

		glm::vec3 boxInCellMin = glm::max(aabb.min, cellMin);
		glm::vec3 boxInCellMax = glm::min(aabb.max, cellMax);

		glm::vec3 overlapSize = glm::max(boxInCellMax - boxInCellMin, glm::vec3(0.0f));
		
		return overlapSize.x * overlapSize.y * overlapSize.z;
		
	}


}