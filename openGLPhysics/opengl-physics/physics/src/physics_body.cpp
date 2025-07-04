#include "headers.h"
#include "physics/include/physics_body.h"
#include "physics/include/physics_manager.h"
#include "tools/include/component_registry.h"
#include "physics/include/aabb.h"
#include "physics/include/obb.h"
#include "physics/include/sphere_bound.h"

#include <glm/gtx/vector_angle.hpp>


namespace physics
{
	PhysicsBody::PhysicsBody(const PhysicsBodyBundleBase& bundle)
	{
		switch (bundle.bound_type())
		{
		case glType::BoundType::AABB:
		{
			const PhysicsBodyBundleAABB& aabbBundle = static_cast<const PhysicsBodyBundleAABB&>(bundle);
			_boundType = std::make_unique<AABB>(aabbBundle.min, aabbBundle.max);
			break;
		}

		case glType::BoundType::OBB:
		{
			const PhysicsBodyBundleOBB& obbBundle = static_cast<const PhysicsBodyBundleOBB&>(bundle);
			_boundType = std::make_unique<OBB>(obbBundle.min, obbBundle.max, obbBundle.rotation);
			break;
		}

		case glType::BoundType::Sphere:
		{
			const PhysicsBodyBundleSphere& sphere = static_cast<const PhysicsBodyBundleSphere&>(bundle);
			_boundType = std::make_unique<SphereBound>(sphere.min, sphere.max, sphere.radius);
			break;
		}
		}

		_netForce = bundle.initalForce;

		_massInv = 1.0f / bundle.mass; 

		_acceleration = _netForce * static_cast<float>(_massInv);

		_volocity = bundle.volocity;

		_gravityAffected = bundle.gravityAffected;

		_addedForce = (_netForce != glm::vec3(0.0f));

		_collisionCallback = bundle.collisionFunc;
	}

	PhysicsBody::PhysicsBody(const PhysicsBody& other)
	{
		_boundType = other._boundType->clone();
		_mesh = other._mesh;
		_netForce = other._netForce;
		_acceleration = other._acceleration;
		_volocity = other._volocity;
		_addedForce = other._addedForce;
		_gravityAffected = other._gravityAffected;
		_massInv = other._massInv;
		_entityId = other._entityId;
	}

	PhysicsBody& PhysicsBody::operator=(const PhysicsBody& other)
	{
		if (this != &other)
		{
			_boundType = other._boundType->clone();
			_mesh = other._mesh;
			_netForce = other._netForce;
			_acceleration = other._acceleration;
			_volocity = other._volocity;
			_addedForce = other._addedForce;
			_gravityAffected = other._gravityAffected;
			_massInv = other._massInv;
			_entityId = other._entityId;
		}
		return *this;
	}


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

	void PhysicsBody::set_volocity(const glm::vec3& val)
	{
		_volocity = val;
		if (_volocity == glm::vec3(0.0f))
		{
			_addedForce = false;
		}
		else
		{
			_addedForce = true;
		}
	}

	glm::vec3 PhysicsBody::get_momentum() const
	{
		return _volocity * static_cast<float>(1.0 / _massInv);
	}

	void PhysicsBody::add_force(const Force& val)
	{
		_netForce += val;

		if (_netForce == glm::vec3(0.0f))
		{
			_addedForce = false;
		}
		else
		{
			_addedForce = true;
		}
	}

	bool PhysicsBody::is_colliding(const PhysicsBody& other) const
	{
		return _boundType->is_touching(other._boundType.get());
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

	void PhysicsBody::collision_response_callback(glType::Entity otherEntity)
	{
		if (_collisionCallback)
		{
			_collisionCallback(otherEntity);
		}
	}

	glType::Entity PhysicsBody::get_entity_id() const

	{
		return _entityId;
	}

	bool PhysicsBody::is_gravity_affected() const
	{
		return _gravityAffected;
	}

	bool PhysicsBody::has_force() const
	{
		return _addedForce;
	}



}
