#pragma once

#include "config.h"

#include "physics/include/bound_base.h"

#include "physics/include/physics_bundles.h"

#include "physics/include/force_input.h"

#include "physics/include/units.h"

#include "glUtil/include/mesh.h"

#include "tools/include/transform.h"



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
		
		PhysType get_type() const;

		void communicate_impl(glType::Entity entity);

		void set_position(const glm::vec3& pos);

		MinMax get_aabb_wrap() const;

		void set_is_touching_ground(bool val);

		bool is_touching_ground() const;

		glm::vec3 get_angular_volocity() const;

		float get_elasticity() const;

		glm::vec3 get_volocity() const;
		
		glm::vec3 get_center_of_mass() const;

		void add_volocity(const glm::vec3& val);

		void set_volocity(const glm::vec3& val);

		void set_angular_volocity(const glm::vec3& val);

		glm::mat3 get_inverse_inertia_tensor() const;

		glm::vec3 get_momentum() const;

		template<ForceType T>
		void add_world_force(const GlobalForce<T>& val);

		void add_force(const Force& val);

		CollisionPoint is_colliding(const PhysicsBody& other) const;

		glm::vec3 get_position() const;

		float get_mass() const;

		float get_volume() const;

		void set_force(const glm::vec3& force);

		float get_inverse_mass() const;

		BoundTypeBase* get_bound_type() const;

		void update(float dt);

		void move_position(const glm::vec3& dist);

		void collision_response_callback(glType::Entity otherEntity);

		glType::Entity get_entity_id() const;

		template<ForceType T>
		ForceCalcInput<T> determine_input_for_force();

		bool is_gravity_affected() const;

		bool has_force() const;

	protected:
		std::unique_ptr<BoundTypeBase> _boundType;

		std::shared_ptr<tools::Transform> _transform;

		glm::vec3 _acceleration;

		glm::vec3 _angularVol;

		glm::vec3 _netForce;

		std::vector<glm::vec3> _appliedForces;

		glm::vec3 _volocity;

		glm::vec3 _prevPos;

		glm::vec3 _centerOfMass;

		glm::mat3 _inverseInertiaMat;

		float _elasticity = 0.5f; 
	
		bool _gravityAffected = false;

		units::Measurible _massInv;

		std::function<void(glType::Entity)> _collisionCallback;

		glType::Entity _entityId;

		bool _isTouchingGround = false;

		PhysType _type = PhysType::Reg;

	protected:
		glm::vec3 apply_forces() const;
	};

	template<ForceType T>
	inline void PhysicsBody::add_world_force(const GlobalForce<T>& val)
	{
		if constexpr (T == ForceType::Gravity)
		{
			if (!_gravityAffected)
				return;
		}

		_appliedForces.emplace_back(val.calc_local_force(determine_input_for_force<T>()));
		_netForce += _appliedForces.back();
		
		if (_netForce != glm::vec3(0.0f))
		{
			_acceleration = _netForce * FLOAT(_massInv);
		}
	}

	template<ForceType T>
	inline ForceCalcInput<T> PhysicsBody::determine_input_for_force()
	{
		ForceCalcInput<T> input;
		if constexpr (T == ForceType::Gravity)
		{
			input.mass = 1.0f / FLOAT(_massInv);
		}
		else if constexpr (T == ForceType::Drag)
		{
			input.volocity = _volocity;
		}
		else if constexpr (T == ForceType::Spring)
		{
			input.displacementPos = _transform->position;
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

