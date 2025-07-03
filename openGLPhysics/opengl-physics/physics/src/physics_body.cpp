#include "headers.h"
#include "physics/include/physics_body.h"
#include "physics/include/spatial_partioning.h"
#include "tools/include/component_registry.h"
#include "glUtil/include/mesh.h"





namespace physics
{

	void PhysicsBody::communicate_impl(glType::Entity entity)
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

	glm::vec3 PhysicsBody::get_volocity() const
	{
		return _volocity;
	}

	void PhysicsBody::add_force(const Force& val)
	{
		_netForce += val;
		_addedForce = true;
	}

	bool PhysicsBody::is_colliding(const PhysicsBody& other) const
	{
		return false;
	}

	glm::vec3 PhysicsBody::get_position() const
	{
		return _mesh->_transform.position;
	}

	MinMax PhysicsBody::get_aabb() const
	{
		return _boundType->get_min_max();
	}

	float PhysicsBody::get_mass() const
	{
		return 1.0f / _massInv;
	}

	float PhysicsBody::get_volume() const
	{
		return _boundType->get_volume();
	}

	void PhysicsBody::set_position(const glm::vec3& pos)
	{
		_mesh->set_position(pos);
	}

	void PhysicsBody::update(float dt)
	{
		if (_addedForce)
		{
			_addedForce = false;
			_acceleration = _netForce * FLOAT(_massInv);
		}

		_volocity += _acceleration * dt;

		_mesh->_transform.position += _volocity;

	}

	glType::Entity PhysicsBody::get_entity_id() const

	{
		return _entityId;
	}



}
