#pragma once


#include "config.h"


#include "tools/include/features.h"

#include "physics/include/units.h"

namespace physics
{

	using namespace units;

	enum class ForceType
	{
		None = 0,
		Gravity = 1 << 0,
		Drag = 1 << 1,
		Spring = 1 << 2,
		Buoyancy = 1 << 3,
		Friction = 1 << 4,
		Normal = 1 << 5,
		Count = 5
	};

	template<ForceType Type>
	struct ForceCalcInput;

	template<>
	struct ForceCalcInput<ForceType::Gravity>
	{
		Measurible mass;
		ForceCalcInput(Measurible mass = 1.0_kilograms) : mass(mass) 
		{};
	};


	template<>
	struct ForceCalcInput<ForceType::Drag>
	{
		glm::vec3 volocity;
		ForceCalcInput(const glm::vec3& volocity = glm::vec3(1.0f)) : volocity(to_meters_per_sec(volocity))
		{
		};
	};


	template<>
	struct ForceCalcInput<ForceType::Spring>
	{
		glm::vec3 compression;
		ForceCalcInput(const glm::vec3& compression = glm::vec3(1.0f))
			: compression(compression)
		{
		};
	};


	template<>
	struct ForceCalcInput<ForceType::Buoyancy>
	{
		Measurible submergedVolume;
		ForceCalcInput(Measurible submergedVolume = 1.0_meters)
			: submergedVolume(submergedVolume)
		{
		};
	};

	using ForceCalcInputBuoyancy = ForceCalcInput<ForceType::Buoyancy>;

	template<>
	struct ForceCalcInput<ForceType::Friction>
	{
		Measurible frictionCoefficient;
		ForceCalcInput(Measurible frictionCoefficient = 0.5_mu)
			: frictionCoefficient(frictionCoefficient)
		{
		};
	};


	template<>
	struct ForceCalcInput<ForceType::Normal>
	{
		Measurible mass;
		Measurible angle;
		ForceCalcInput(Measurible mass = 1.0_kilograms, Measurible angle = 0.0_degrees)
			: mass(mass), angle(angle)
		{
		}
	};

}


namespace glType
{
	using ForceCalcInputNormal = physics::ForceCalcInput<physics::ForceType::Normal>;
	using ForceCalcInputFriction = physics::ForceCalcInput<physics::ForceType::Friction>;
	using ForceCalcInputDrag = physics::ForceCalcInput<physics::ForceType::Drag>;
	using ForceCalcInputSpring = physics::ForceCalcInput<physics::ForceType::Spring>;
	using ForceCalcInputGravity = physics::ForceCalcInput<physics::ForceType::Gravity>;
}