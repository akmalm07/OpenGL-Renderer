#include "headers.h"
#include "physics/include/force.h"

namespace physics
{
	GravityForce::GravityForce(const glm::vec3& gravity)
		: acc(to_meters_per_sec2(gravity))
	{
	}

	Force GravityForce::calc_local_force_impl(const ForceCalcInput<ForceType::Gravity>& in) const
	{
		return FLOAT(in.mass) * acc;
	}


	DragForce::DragForce(Measurible dragCoefficient, Measurible fluidDensity, Measurible area)
		: dragCoefficient(dragCoefficient), fluidDensity(fluidDensity), area(area)
	{
	}

	Force DragForce::calc_local_force_impl(const ForceCalcInput<ForceType::Drag>& in) const
	{
		return -0.5f * FLOAT(dragCoefficient * fluidDensity * area) * in.volocity * in.volocity;
	}

	SpringForce::SpringForce(Measurible springConstant, const glm::vec3& restPos)
		: springConstant(springConstant), restPos(to_meters(restPos))
	{
	}

	Force SpringForce::calc_local_force_impl(const ForceCalcInput<ForceType::Spring>& in) const
	{
		return FLOAT(springConstant) * (in.displacementPos - restPos);
	}

	BuoyancyForce::BuoyancyForce(Measurible fluidDensity, const glm::vec3& gravitationalAcc)
		: fluidDensity(fluidDensity), acc(to_meters_per_sec2(gravitationalAcc))
	{
	}

	Force BuoyancyForce::calc_local_force_impl(const ForceCalcInput<ForceType::Buoyancy>& in) const
	{
		return FLOAT(fluidDensity * in.submergedVolume) * acc * glm::vec3(0.0, 1.0, 0.0);
	}



	FrictionForce::FrictionForce(Measurible frictionCoefficient)
		: frictionCoefficient(frictionCoefficient)
	{
	}

	Force FrictionForce::calc_local_force_impl(const ForceCalcInput<ForceType::Friction>& in) const
	{
		return -(FLOAT(frictionCoefficient * in.normalForce) * glm::normalize(in.volocity));
	}

	NormalForce::NormalForce(const glm::vec3& gravitationalAcc)
		: acc(to_meters_per_sec2(gravitationalAcc))
	{
	}

	Force NormalForce::calc_local_force_impl(const ForceCalcInput<ForceType::Normal>& in) const
	{
		return -(FLOAT(glm::cos(glm::radians(in.angle)) * in.mass) * acc);
	}


	/*
	Force GlobalForceBase::calc_local_force(const ForceCalcInputBase& type) const
	{
		return static_cast<const GlobalForce<>*>(this);
	}
	*/

}
