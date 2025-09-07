#pragma once

#include "physics/include/physics_manager.h"

#include "physics/include/physics_body.h"

#include "physics/include/bound_base.h"



namespace physics
{

    template<size_t CellCount>
    inline PhysicsManager<CellCount>::PhysicsManager(tools::ComponentRegistry<PhysicsBody>& physBodyComponentInstance)
    : _physBodyComponentInstance(physBodyComponentInstance)
    {
    }

	template<size_t CellCount>
	PhysicsManager<CellCount>::PhysicsManager(tools::ComponentRegistry<PhysicsBody>& physBodyComponentInstance, const glm::vec3& minBound, const glm::vec3& maxBound)
		: _cellWidth((maxBound.x - minBound.x) / CellCount),
		_cellHeight((maxBound.y - minBound.y) / CellCount),
		_physBodyComponentInstance(physBodyComponentInstance)
	 {
		for (size_t i = 0; i < CellCount; i++)
		{
			_cellMinBounds[i] = glm::vec3(minBound.x + i * _cellWidth, minBound.y, minBound.z);
			_cellMaxBounds[i] = glm::vec3(minBound.x + (i + 1) * _cellWidth, maxBound.y, maxBound.z);
		}
	}

	template<size_t CellCount>
	inline void PhysicsManager<CellCount>::register_body(const PhysicsBody& body)
	{
		MinMax position = body.get_aabb_wrap();
		
		//bool firstTime = true;
		
		for (size_t i = 0; i < CellCount; i++)
		{
			const glm::vec3& cellMin = _cellMinBounds[i];
			const glm::vec3& cellMax = _cellMaxBounds[i];


			if (position.max.x >= cellMin.x && position.min.x <= cellMax.x &&
				position.max.y >= cellMin.y && position.min.y <= cellMax.y &&
				position.max.z >= cellMin.z && position.min.z <= cellMax.z)
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

		for (size_t i = 0; i < _participatingEntities.ents.size(); i++)
		{
			PhysicsBody* body = _physBodyComponentInstance.get_component_or_null(_participatingEntities.ents[i]);

			if (!body || !body->has_force())
			{
				continue;
			}


			size_t initialVal = (_participatingEntities.index[i] == 0 ? 0 : _participatingEntities.index[i] - 1);
			float totalCollectedVol = 0.0f;

			// Forward Scan
			for (size_t j = initialVal; j < CellCount; j++)
			{
				totalCollectedVol += run_check_of_body_index(body, j, i);
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
				totalCollectedVol += run_check_of_body_index(body, j, i);
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

		for (size_t i = 0; i < CellCount; i++)
		{

			if (_cells[i].size() <= 1)
			{
				continue;
			}

			for (size_t j = 0; j < _cells[i].size(); j++)
			{
				PhysicsBody* physBody = _physBodyComponentInstance.get_component_or_null(_cells[i][j]);

				if (!physBody)
					continue;

				for (size_t k = j + 1; k < _cells[i].size(); k++) // O(n^2) time
				{
					PhysicsBody* other = _physBodyComponentInstance.get_component_or_null(_cells[i][k]);
					
					if (!other)
						continue;

					CollisionPoint data = physBody->is_colliding(*other);
					
					if (data.exists)
					{
						collision_response(physBody, other, data);
					}
				}
			}
		}
	}


	template<size_t CellCount>
	inline void PhysicsManager<CellCount>::collision_response(PhysicsBody* body, PhysicsBody* other, const CollisionPoint& collisionPoint) const 
	{

		//// Separate them out using projection
		float totalMass = body->get_inverse_mass() + other->get_inverse_mass();

		if (body->get_type() == PhysType::Floor)
		{	
			other->set_is_touching_ground(true);
			return;
		}
		else if (other->get_type() == PhysType::Floor)
		{
			body->set_is_touching_ground(true);
			return;
		}

		
		body->set_position(body->get_position() - (collisionPoint.normal * collisionPoint.penetrationDepth * (body->get_inverse_mass() / totalMass)));
		other->set_position(other->get_position() - (collisionPoint.normal * collisionPoint.penetrationDepth * (other->get_inverse_mass() / totalMass)));
		

		glm::vec3 relativePosA = collisionPoint.position - body->get_position();
		glm::vec3 relativePosB = collisionPoint.position - other->get_position();


		glm::vec3 angVelocityA = glm::cross(body->get_angular_volocity(), relativePosA);
		glm::vec3 angVelocityB = glm::cross(other->get_angular_volocity(), relativePosB);

		glm::vec3 totalVelocityA = body->get_volocity() + angVelocityA;
		glm::vec3 totalVelocityB = other->get_volocity() + angVelocityB;
		glm::vec3 totalRelativeVelocity = totalVelocityB - totalVelocityA;

		float impulseForce = glm::dot(totalRelativeVelocity, collisionPoint.normal);

		if (impulseForce > 0.0f) 
			return;

		float elasticity = std::min(body->get_elasticity(), other->get_elasticity());

		glm::vec3 raCrossN = glm::cross(relativePosA, collisionPoint.normal);
		glm::vec3 rbCrossN = glm::cross(relativePosB, collisionPoint.normal);

		float invInertiaA = glm::dot(raCrossN, body->get_inverse_inertia_tensor() * raCrossN);
		float invInertiaB = glm::dot(rbCrossN, other->get_inverse_inertia_tensor() * rbCrossN);

		float denominator = body->get_inverse_mass() + other->get_inverse_mass() + invInertiaA + invInertiaB;
		
		float j = -(1.0f + elasticity) * impulseForce / denominator;

		body->set_volocity(body->get_volocity() - (j * body->get_inverse_mass()) * collisionPoint.normal);
		other->set_volocity(other->get_volocity() + (j * other->get_inverse_mass()) * collisionPoint.normal);

		body->set_angular_volocity(body->get_angular_volocity() - body->get_inverse_inertia_tensor() * glm::cross(relativePosA, j * collisionPoint.normal));
		other->set_angular_volocity(other->get_angular_volocity() + other->get_inverse_inertia_tensor() * glm::cross(relativePosB, j * collisionPoint.normal));

		// User-defined callbacks
		body->collision_response_callback(other->get_entity_id());
		other->collision_response_callback(body->get_entity_id());
	}

	//{
	//	std::cout << "Collision detected between entities: " << body->get_entity_id() << " and " << other->get_entity_id() << std::endl;


	//	glm::vec3 relativeVelocity = other->get_volocity() - body->get_volocity();
	//	glm::vec3 normal = glm::normalize(relativeVelocity);

	//	float velocityAlongNormal = glm::dot(relativeVelocity, normal);

	//	// Skip if objects are moving apart
	//	if (velocityAlongNormal > 0.0f)
	//		return;

	//	float elasticity = std::min(body->get_elasticity(), other->get_elasticity()); // For elasticity

	//	if (std::abs(velocityAlongNormal) < 0.1f)
	//		elasticity = 0.0f;

	//	// Calculate impulse scalar
	//	float impulseMagnitude = -(1.0f + elasticity) * velocityAlongNormal / (body->get_inverse_mass() + other->get_inverse_mass());

	//	glm::vec3 impulse = impulseMagnitude * normal;

	//	// Apply impulse
	//	body->set_volocity(body->get_volocity() - body->get_inverse_mass() * impulse);
	//	other->set_volocity(other->get_volocity() + other->get_inverse_mass() * impulse);


	//	if (glm::length(body->get_volocity()) < 0.01f)
	//		body->set_volocity(glm::vec3(0.0f));

	//	if (glm::length(other->get_volocity()) < 0.01f)
	//		other->set_volocity(glm::vec3(0.0f));

	//	// User-defined callbacks
	//	body->collision_response_callback(other->get_entity_id());
	//	other->collision_response_callback(body->get_entity_id());
	//}


	template<size_t CellCount>
	inline float PhysicsManager<CellCount>::run_check_of_body_index(PhysicsBody* body, size_t index, size_t entityIndex)
	{
		const glm::vec3& cellMin = _cellMinBounds[index];
		const glm::vec3& cellMax = _cellMaxBounds[index];
		
		MinMax position = body->get_aabb_wrap();

		auto it = std::find(_cells[index].begin(), _cells[index].end(), body->get_entity_id());
		
		
		if (position.max.x >= cellMin.x && position.min.x <= cellMax.x &&
			position.max.y >= cellMin.y && position.min.y <= cellMax.y &&
			position.max.z >= cellMin.z && position.min.z <= cellMax.z)
		{
			if (it == _cells[index].end())
			{
				_cells[index].push_back(body->get_entity_id());
			}

			_participatingEntities.index[entityIndex] = index;

			return get_vol_of_entity_in_cell(body, index);
		}
		else if (it == _cells[index].end())
		{
			return 0.0f;
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

		MinMax aabb = body->get_aabb_wrap();

		glm::vec3 cellMin = _cellMinBounds[index];
		glm::vec3 cellMax = _cellMaxBounds[index];

		glm::vec3 boxInCellMin = glm::max(aabb.min, cellMin);
		glm::vec3 boxInCellMax = glm::min(aabb.max, cellMax);

		glm::vec3 overlapSize = glm::max(boxInCellMax - boxInCellMin, glm::vec3(0.0f));
		
		return overlapSize.x * overlapSize.y * overlapSize.z;
		
	}


}