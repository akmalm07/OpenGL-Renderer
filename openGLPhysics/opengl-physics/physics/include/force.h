#pragma once

#include "config.h"

#include "physics/include/acceleration.h"

#include "physics/include/force_input.h"

#include "physics/include/units.h"

namespace physics
{
	using namespace units;

	template <ForceType Type>
	class GlobalForce;

	template<typename T, ForceType Enum>
	concept HasCalcForceImpl =
		std::is_base_of_v<GlobalForce<Enum>, T>&&
		requires(const T& t) {
			{ t.calc_local_force_impl(std::declval<const ForceCalcInput<Enum>&>()) } -> std::same_as<Force>;
	};

	class GlobalForceBase
	{
	public:
		virtual ~GlobalForceBase() = default;

		template<ForceType Type>
		Force calc_local_force(const ForceCalcInput<Type>& type) const;
	};

	template <ForceType Type>
	class GlobalForce : public GlobalForceBase
	{
	public:
		
		Force calc_local_force(const ForceCalcInput<Type>& type) const requires HasCalcForceImpl<GlobalForce<Type>, Type>;

	protected:
	};


	class GravityForce : public GlobalForce<ForceType::Gravity>
	{
	public:
		GravityForce(const glm::vec3& gravity = glm::vec3(0.0, -9.81, 0.0));
		Force calc_local_force_impl(const ForceCalcInput<ForceType::Gravity>& in) const;
	private:
		glm::vec3 acc;
	};

	class DragForce : public GlobalForce<ForceType::Drag>
	{
	public:
		DragForce(Measurible dragCoefficient = 0.47_mu, Measurible fluidDensity = 1.0_kg_per_meter3, Measurible area = 10.0_meter2);
		Force calc_local_force_impl(const ForceCalcInput<ForceType::Drag>& in) const;

	private:
		Measurible dragCoefficient;
		Measurible fluidDensity;
		Measurible area;
	};

	class SpringForce : public GlobalForce<ForceType::Spring>
	{
	public:
		SpringForce(Measurible springConstant = 1.0_kg_per_meter2, const glm::vec3& restPos = glm::vec3(1.0f));
		Force calc_local_force_impl(const ForceCalcInput<ForceType::Spring>& in) const;
	private:
		Measurible springConstant;
		glm::vec3 restPos;
	};

	class BuoyancyForce : public GlobalForce<ForceType::Buoyancy>
	{
	public:
		BuoyancyForce(Measurible fluidDensity = 1.0_kg_per_meter3, const glm::vec3& gravitationalAcc = glm::vec3(0.0, -9.81, 0.0));
		Force calc_local_force_impl(const ForceCalcInput<ForceType::Buoyancy>& in) const;
	private:
		Measurible fluidDensity;
		glm::vec3 acc;
	};
	
	class FrictionForce : public GlobalForce<ForceType::Friction>
	{
	public:
		FrictionForce(Measurible frictionCoefficient = 0.5_mu);
		Force calc_local_force_impl(const ForceCalcInput<ForceType::Friction>& in) const;
	private:
		Measurible frictionCoefficient;
	};


	class NormalForce : public GlobalForce<ForceType::Normal>
	{
	public:
		NormalForce(const glm::vec3& gravitationalAcc = glm::vec3(0.0, -9.81, 0.0));

		Force calc_local_force_impl(const ForceCalcInput<ForceType::Normal>& in) const;

	private:
		glm::vec3 acc;
	};


	template<ForceType Type>
	inline Force GlobalForce<Type>::calc_local_force(const ForceCalcInput<Type>& type) const requires HasCalcForceImpl<GlobalForce<Type>, Type>
	{
		return	static_cast<const GlobalForce<Type>*>(this)->calc_local_force_impl(type);
	}


	

	template<ForceType Type>
	inline Force GlobalForceBase::calc_local_force(const ForceCalcInput<Type>& type) const
	{
		return GlobalForce<Type>::calc_local_force(type);
	}

}

