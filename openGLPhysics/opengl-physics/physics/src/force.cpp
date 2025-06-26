#include "headers.h"
#include "physics/include/force.h"

namespace physics
{
	GravityForce::GravityForce(Measurible gravity)
	{
		_acc = gravity;
	}

	Force GravityForce::calc_local_force_impl(ForceCalcInput<ForceType::Gravity> in) const
	{
		return Force(0.0f, _acc, 0.0f) * static_cast<float>(in.mass);
	}


	DragForce::DragForce(Measurible dragCoefficient, Measurible fluidDensity, Measurible area)
	{
		this->dragCoefficient = dragCoefficient;
		this->fluidDensity = fluidDensity;
		this->area = area;
	}

	Force DragForce::calc_local_force_impl(ForceCalcInput<ForceType::Drag> in) const
	{
		return -0.5f * static_cast<float>(dragCoefficient * fluidDensity * area) * in.volocity * in.volocity;
	}

	SpringForce::SpringForce(Measurible springConstant, Measurible restLength)
	{
		this->springConstant = springConstant;
		this->restLength = restLength;
	}

	Force SpringForce::calc_local_force_impl(ForceCalcInput<ForceType::Spring> in) const
	{
		return -static_cast<float>(springConstant * (in.compression - restLength));
	}

}
