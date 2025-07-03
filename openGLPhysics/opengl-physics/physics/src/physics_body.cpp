#include "headers.h"
#include "physics/include/physics_body.h"
#include "physics/include/spatial_partioning.h"
#include "tools/include/component_registry.h"
#include "glUtil/include/mesh.h"





namespace physics
{

	void PhysicsBodyBase::communicate_impl(glType::Entity entity)
	{
		auto& registry = tools::ComponentRegistry<glUtil::Mesh>::get_instance();
		auto& registered = registry.get_entities();

		_entityId = entity;

		auto it = registered.find(entity);

		if (it != registered.end())
		{
			_mesh = it->second;
		}
		else
		{
			registry.add_component(entity, tools::construct_default_mesh());

			_mesh = registered[entity];
		}
	}

	glm::vec3 PhysicsBodyBase::get_volocity() const
	{
		return _volocity;
	}

	void PhysicsBodyBase::add_force(const Force& val)
	{
		_netForce += val;
		_addedForce = true;
	}

	bool PhysicsBodyBase::is_colliding(const PhysicsBodyBase& other) const
	{
		return false;
	}

	glm::vec3 PhysicsBodyBase::get_position() const
	{
		return _mesh->_transform.position;
	}

	MinMax PhysicsBodyBase::get_aabb() const
	{
		return _boundType->get_min_max();
	}

	float PhysicsBodyBase::get_mass() const
	{
		return 1.0f / _massInv;
	}

	float PhysicsBodyBase::get_volume() const
	{
		return _boundType->get_volume();
	}

	void PhysicsBodyBase::set_position(const glm::vec3& pos)
	{
		_mesh->set_position(pos);
	}

	void PhysicsBodyBase::update(float dt)
	{
		if (_addedForce)
		{
			_addedForce = false;
			_acceleration = _netForce * FLOAT(_massInv);
		}

		_volocity += _acceleration * dt;

		_mesh->_transform.position += _volocity;

	}

	glType::Entity PhysicsBodyBase::get_entity_id() const

	{
		return _entityId;
	}



}
