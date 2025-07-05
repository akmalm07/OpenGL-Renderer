#pragma once

#include "physics/include/force.h"

namespace physics
{
	inline GlobalForce<ForceType::Gravity>::GlobalForce(const glm::vec3& gravity)
		: acc(to_meters_per_sec2(gravity))
	{
	}

	inline Force GlobalForce<ForceType::Gravity>::calc_local_force(const ForceCalcInput<ForceType::Gravity>& in) const
	{
		return FLOAT(in.mass) * acc;
	}


	inline GlobalForce<ForceType::Drag>::GlobalForce(Measurible dragCoefficient, Measurible fluidDensity, Measurible area)
		: dragCoefficient(dragCoefficient), fluidDensity(fluidDensity), area(area)
	{
	}

	inline Force GlobalForce<ForceType::Drag>::calc_local_force(const ForceCalcInput<ForceType::Drag>& in) const
	{
		return -0.5f * FLOAT(dragCoefficient * fluidDensity * area) * in.volocity * in.volocity;
	}

	inline GlobalForce<ForceType::Spring>::GlobalForce(Measurible springConstant, const glm::vec3& restPos)
		: springConstant(springConstant), restPos(to_meters(restPos))
	{
	}

	inline Force GlobalForce<ForceType::Spring>::calc_local_force(const ForceCalcInput<ForceType::Spring>& in) const
	{
		return FLOAT(springConstant) * (in.displacementPos - restPos);
	}

	inline GlobalForce<ForceType::Buoyancy>::GlobalForce(Measurible fluidDensity, const glm::vec3& gravitationalAcc)
		: fluidDensity(fluidDensity), acc(to_meters_per_sec2(gravitationalAcc))
	{
	}

	inline Force GlobalForce<ForceType::Buoyancy>::calc_local_force(const ForceCalcInput<ForceType::Buoyancy>& in) const
	{
		return FLOAT(fluidDensity * in.submergedVolume) * acc * glm::vec3(0.0, 1.0, 0.0);
	}



	inline GlobalForce<ForceType::Friction>::GlobalForce(Measurible frictionCoefficient)
		: frictionCoefficient(frictionCoefficient)
	{
	}

	inline Force GlobalForce<ForceType::Friction>::calc_local_force(const ForceCalcInput<ForceType::Friction>& in) const
	{
		return -(FLOAT(frictionCoefficient * in.normalForce) * glm::normalize(in.volocity));
	}

	inline GlobalForce<ForceType::Normal>::GlobalForce(const glm::vec3& gravitationalAcc)
		: acc(to_meters_per_sec2(gravitationalAcc))
	{
	}

	inline Force GlobalForce<ForceType::Normal>::calc_local_force(const ForceCalcInput<ForceType::Normal>& in) const
	{
		return -(FLOAT(glm::cos(glm::radians(in.angle)) * in.mass) * acc);
	}

}
