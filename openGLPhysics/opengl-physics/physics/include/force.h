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

	using Force = glm::vec3;

	template<typename T, ForceType Enum>
	concept HasCalcForceImpl =
		std::is_base_of_v<GlobalForce<Enum>, T>&&
		requires(const T& t) {
			{ t.calc_local_force_impl(std::declval<ForceCalcInput<Enum>>()) } -> std::same_as<Force>;
	};

	template <ForceType Type>
	class GlobalForce
	{
	public:
		
		Force calc_local_force(ForceCalcInput<Type> type) const requires HasCalcForceImpl<GlobalForce<Type>, Type>;

	protected:
	};


	class GravityForce : public GlobalForce<ForceType::Gravity>
	{
	public:
		GravityForce(Measurible gravity = -9.81_meters_per_sec2);
		Force calc_local_force_impl(ForceCalcInput<ForceType::Gravity> in) const;
	private:
		Measurible _acc; 
	};

	class DragForce : public GlobalForce<ForceType::Drag>
	{
	public:
		DragForce(Measurible dragCoefficient = 0.47_mu, Measurible fluidDensity = 1.0_kg_per_meter3, Measurible area = 10.0_meter2);
		Force calc_local_force_impl(ForceCalcInput<ForceType::Drag> in) const;

	private:
		Measurible dragCoefficient;
		Measurible fluidDensity;
		Measurible area;
	};

	class SpringForce : public GlobalForce<ForceType::Spring>
	{
	public:
		SpringForce(Measurible springConstant = 1.0_kg_per_meter2, Measurible restLength = 1.0_meters);
		Force calc_local_force_impl(ForceCalcInput<ForceType::Spring> in) const;
	private:
		Measurible springConstant;
		Measurible restLength;
	};
}
