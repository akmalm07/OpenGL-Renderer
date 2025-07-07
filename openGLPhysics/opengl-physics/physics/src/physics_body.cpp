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

		_prevPos = glm::vec3(0.0f);

		_centerOfMass = bundle.centerOfMass;

		_gravityAffected = bundle.gravityAffected;

		_addedForce = (_netForce != glm::vec3(0.0f));

		_elasticity = glm::clamp(bundle.elasticity, 0.0f, 1.0f);

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
		_collisionCallback = other._collisionCallback;
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
			_collisionCallback = other._collisionCallback;
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

	float PhysicsBody::get_elasticity() const
	{
		return _elasticity;
	}

	glm::vec3 PhysicsBody::get_volocity() const
	{
		return _volocity;
	}

	glm::vec3 PhysicsBody::get_center_of_mass() const
	{
		return _centerOfMass;
	}

	void PhysicsBody::add_volocity(const glm::vec3& val)
	{
		_volocity += val;
		if (_volocity == glm::vec3(0.0f))
		{
			_addedForce = false;
		}
		else
		{
			_addedForce = true;
		}
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
		_appliedForces.emplace_back(val);
		_netForce += val;

		if (_netForce == glm::vec3(0.0f))
		{
			_addedForce = false;
		}
		else
		{
			_acceleration = _netForce * FLOAT(_massInv);
			_addedForce = true;
		}
	}

	TouchingData PhysicsBody::is_colliding(const PhysicsBody& other) const
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

	void PhysicsBody::set_force(const glm::vec3& force)
	{
		_netForce = force;
		if (_netForce == glm::vec3(0.0f))
		{
			_addedForce = false;
		}
		else
		{
			_addedForce = true;
			_acceleration = _netForce * FLOAT(_massInv);
		}
	}

	float PhysicsBody::get_inverse_mass() const
	{
		return _massInv;
	}

	BoundTypeBase* PhysicsBody::get_bound_type() const
	{
		return _boundType.get();
	}

	void PhysicsBody::set_position(const glm::vec3& pos)
	{
		_mesh->set_position(pos);
	}

	void PhysicsBody::update(float dt)
	{
		if (_addedForce)
		{
			_netForce = apply_forces();
			_acceleration = _netForce * FLOAT(_massInv);
		}

		_volocity += _acceleration * dt;

		if (_addedForce && glm::length(_volocity) < 0.01f)
		{
			_volocity = glm::vec3(0.0f);
			_acceleration = glm::vec3(0.0f);
			_addedForce = false;
		}

		_prevPos = _mesh->_transform.position;

		_mesh->_transform.position += _volocity * dt;

		_centerOfMass += _volocity * dt;


		switch (_boundType->get_bound_type())
		{
		case glType::BoundType::AABB:
		case glType::BoundType::Sphere:
		{
			_boundType->change(_mesh->_transform.position - _prevPos);
		}
			break;
		case glType::BoundType::OBB:
		{
			static_cast<OBB*>(_boundType.get())->move(_mesh->_transform.position - _prevPos);
		}
		break;
		}



	}

	void PhysicsBody::move_position(const glm::vec3& dist)
	{
		_mesh->_transform.position += dist;
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

	glm::vec3 PhysicsBody::apply_forces() const
	{
		glm::vec3 netForce = glm::vec3(0.0f);
		for (const auto& force : _appliedForces)
		{
			netForce += force;
		}

		return netForce;
	}


}
