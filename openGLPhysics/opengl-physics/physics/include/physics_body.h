#pragma once

#include "config.h"

#include "physics/include/aabb.h"

#include "physics/include/obb.h"

#include "physics/include/acceleration.h"

#include "physics/include/volocity.h"

#include "physics/include/force.h"

#include "tools/include/features.h"

#include "physics/include/units.h"

#include "tools/include/compact_map.h"

#include <glm/gtx/vector_angle.hpp>


namespace glUtil
{
	class Mesh;
}


namespace physics
{

	class SpatialPartioningBase;

	template<size_t T>
	class SpatialPartioning;



	class PhysicsBodyBase : public glType::Component<PhysicsBodyBase>
	{
	public:

		void communicate_impl(glType::Entity entity);

		glm::vec3 get_volocity() const;

		template<ForceType T>
		void add_external_force(const GlobalForce<T>& val);

		void add_force(const Force& val);

		bool is_colliding(const PhysicsBodyBase& other) const;

		glm::vec3 get_position() const;

		MinMax get_aabb() const;

		float get_mass() const;

		float get_volume() const;

		void set_position(const glm::vec3& pos);

		void update(float dt);

		glType::Entity get_entity_id() const;

		template<ForceType T>
		ForceCalcInput<T> determine_input_for_force();

		template<size_t T>
		void add_to_partioner(SpatialPartioning<T>& partioner);

	private:
		std::unique_ptr<BoundTypeBase> _boundType;

		glUtil::Mesh* _mesh; 

		Force _netForce;

		glm::vec3 _acceleration;
		
		glm::vec3 _volocity;

		bool _addedForce = false;
	
		Measurible _massInv;

		glType::Entity _entityId;
	};

	template<ForceType T>
	inline void PhysicsBodyBase::add_external_force(const GlobalForce<T>& val)
	{
		_netForce += val.calc_local_force(determine_input_for_force<T>());
		_addedForce = true;
	}

	template<ForceType T>
	inline ForceCalcInput<T> PhysicsBodyBase::determine_input_for_force()
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
		else
		{
			static_assert(tools::has(T, tools::PhysBodyFeatures::Force), "Unsupported Force Type");
		}

		return input;
	}

	template<size_t T>
	inline void PhysicsBodyBase::add_to_partioner(SpatialPartioning<T>& partioner)
	{
		partioner.register_body(this);
	}

}




//struct MassComponent
//{
//protected:
//	Measurible _mass; 
//};

//struct AABBComponent
//{
//protected:
//	AABB _aabb; // Axis-Aligned Bounding Box
//};

//struct VelocityComponent
//{
//protected:
//	Volocity _volocity; // Represents the velocity of the body
//};

//struct ForceComponent
//{
//protected:
//	Acceleration _force; // Represents the force acting on the body
//};

//struct Empty
//{
//};


/*
* Get Current Vol
* Add Force
* IsColliding
* Spatial Position
* Add Collision Callback(PhysicsBody)
*  - GeneralReation


*/


//template<tools::PhysBodyFeatures Features>
//class PhysicsBody : 
//	public glType::Component<PhysicsBody<Features>>,
//	
//	public std::conditional_t<has(Features, tools::PhysBodyFeatures::AABB), AABBComponent, Empty>,
//	public std::conditional_t<has(Features, tools::PhysBodyFeatures::Volocity), VelocityComponent, Empty>,
//	public std::conditional_t<has(Features, tools::PhysBodyFeatures::Force), ForceComponent, Empty>,
//	public std::conditional_t<has(Features, tools::PhysBodyFeatures::Mass), MassComponent, Empty>
//{
//public:
//	PhysicsBody() = default;



//private:
//	tools::view_ptr_mutable<glUtil::Mesh> _mesh;
//};