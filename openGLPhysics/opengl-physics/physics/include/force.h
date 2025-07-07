#pragma once

#include "config.h"

#include "physics/include/force_input.h"

#include "physics/include/units.h"

namespace physics
{
	using namespace units;



	template <ForceType Type>
	class GlobalForce;


	template<>
	class GlobalForce <ForceType::Gravity> 
	{
	public:
		GlobalForce(const glm::vec3& gravity = glm::vec3(0.0f, -9.0f, 0.0f));
		Force calc_local_force(const ForceCalcInput<ForceType::Gravity>& in) const;
	private:
		glm::vec3 acc;
	};


	template<>
	class GlobalForce <ForceType::Drag> 
	{
	public:
		GlobalForce(Measurible dragCoefficient = 0.47_mu, Measurible fluidDensity = 1.0_kg_per_meter3, Measurible area = 10.0_meter2);
		Force calc_local_force(const ForceCalcInput<ForceType::Drag>& in) const;

	private:
		Measurible dragCoefficient;
		Measurible fluidDensity;
		Measurible area;
	};

	template<>
	class GlobalForce <ForceType::Spring> 
	{
	public:
		GlobalForce(Measurible springConstant = 1.0_kg_per_meter2, const glm::vec3& restPos = glm::vec3(1.0f));
		Force calc_local_force(const ForceCalcInput<ForceType::Spring>& in) const;
	private:
		Measurible springConstant;
		glm::vec3 restPos;
	};


	template <>
	class GlobalForce <ForceType::Buoyancy> 
	{
	public:
		GlobalForce(Measurible fluidDensity = 1.0_kg_per_meter3, const glm::vec3& gravitationalAcc = glm::vec3(0.0, -9.81, 0.0));
		Force calc_local_force(const ForceCalcInput<ForceType::Buoyancy>& in) const;
	private:
		Measurible fluidDensity;
		glm::vec3 acc;
	};
	
	template<>
	class GlobalForce <ForceType::Friction> 
	{
	public:
		GlobalForce(Measurible frictionCoefficient = 0.5_mu);
		Force calc_local_force(const ForceCalcInput<ForceType::Friction>& in) const;
	private:
		Measurible frictionCoefficient;
	};


	template<>
	class GlobalForce <ForceType::Normal> 
	{
	public:
		GlobalForce(const glm::vec3& gravitationalAcc = glm::vec3(0.0, -9.81, 0.0));

		Force calc_local_force(const ForceCalcInput<ForceType::Normal>& in) const;

	private:
		glm::vec3 acc;
	};



//

}


#include "physics/include/force.inl"



//class GlobalForceBase
	//{
	//public:
	//	virtual ~GlobalForceBase() = default;
	//
	//	template<ForceType Type>
	//	Force calc_local_force(const ForceCalcInput<Type>& type) const;
	//};
	//	template<ForceType Type>
	//	inline Force GlobalForceBase::calc_local_force(const ForceCalcInput<Type>& type) const
	//	{
	//		return static_cast<GlobalForce<Type>>(this)->calc_local_force(type);
	//	}