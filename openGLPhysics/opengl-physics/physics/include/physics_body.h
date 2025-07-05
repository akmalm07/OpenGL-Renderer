#pragma once

#include "config.h"

#include "physics/include/bound_base.h"

#include "physics/include/physics_bundles.h"

#include "physics/include/force_input.h"

#include "physics/include/units.h"

#include "glUtil/include/mesh.h"



namespace physics
{

	class PhysicsManagerBase;

	template<size_t T>
	class PhysicsManager;

	template <ForceType T>
	class GlobalForce;

	class PhysicsBody : public glType::Component<PhysicsBody>
	{
	public:
		
		PhysicsBody() = default;

		PhysicsBody(const PhysicsBodyBundleBase& bundle);

		PhysicsBody(const PhysicsBody& other);
		PhysicsBody& operator=(const PhysicsBody& other);

		PhysicsBody(PhysicsBody&&) = default;
		PhysicsBody& operator=(PhysicsBody&&) = default;

		void communicate_impl(glType::Entity entity);

		float get_elasticity() const;

		glm::vec3 get_volocity() const;

		void set_volocity(const glm::vec3& val);

		glm::vec3 get_momentum() const;

		template<ForceType T>
		void add_external_force(const GlobalForce<T>& val);

		void add_force(const Force& val);

		bool is_colliding(const PhysicsBody& other) const;

		glm::vec3 get_position() const;

		MinMax get_aabb() const;

		float get_mass() const;

		float get_volume() const;

		void set_position(const glm::vec3& pos);

		void update(float dt);

		void collision_response_callback(glType::Entity otherEntity);

		glType::Entity get_entity_id() const;

		template<ForceType T>
		ForceCalcInput<T> determine_input_for_force();

		bool is_gravity_affected() const;

		bool has_force() const;

	protected:
		std::unique_ptr<BoundTypeBase> _boundType;

		glUtil::Mesh* _mesh; 

		Force _netForce;

		glm::vec3 _acceleration;

		glm::vec3 _volocity;

		glm::vec3 _prevPos;

		float _elasticity = 0.5f; 

		bool _addedForce = false;
	
		bool _gravityAffected = false;

		units::Measurible _massInv;

		std::function<void(glType::Entity)> _collisionCallback;

		glType::Entity _entityId;
	};

	template<ForceType T>
	inline void PhysicsBody::add_external_force(const GlobalForce<T>& val)
	{
		if constexpr (T == ForceType::Gravity)
		{
			if (!_gravityAffected)
				return;
		}

		glm::vec3 added = val.calc_local_force(determine_input_for_force<T>());
		_netForce += added;
		_addedForce = true;
	}

	template<ForceType T>
	inline ForceCalcInput<T> PhysicsBody::determine_input_for_force()
	{
		ForceCalcInput<T> input;
		if constexpr (T == ForceType::Gravity)
		{
			input.mass = 1.0f / _massInv;
		}
		else if constexpr (T == ForceType::Drag)
		{
			input.volocity = _volocity;
		}
		else if constexpr (T == ForceType::Spring)
		{
			input.displacementPos = _mesh->_transform.position;
		}
		else if constexpr (T == ForceType::Buoyancy)
		{
			input.submergedVolume = _boundType->get_volume();
		}
		else if constexpr (T == ForceType::Friction)
		{
			input.normalForce = _netForce;
			input.volocity = _volocity;
		}
		else if constexpr (T == ForceType::Normal)
		{
			input.mass =  1.0f / _massInv;
			input.angle = glm::degrees(glm::angle(glm::vec3(0.0f, -1.0f, 0.0f), glm::normalize(_volocity)));
		}


		return input;
	}

}

