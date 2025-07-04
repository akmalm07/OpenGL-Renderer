#pragma once


#include "config.h"


#include "physics/include/units.h"

namespace physics
{

	using namespace units;

	using Force = glm::vec3;

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

	struct ForceCalcInputBase
	{

	};

	template<ForceType Type>
	struct ForceCalcInput : ForceCalcInputBase
	{};

	template<>
	struct ForceCalcInput<ForceType::Gravity> : ForceCalcInputBase
	{
		Measurible mass;
		ForceCalcInput(Measurible mass = 1.0_kilograms) : mass(mass) 
		{};
	};


	template<>
	struct ForceCalcInput<ForceType::Drag> : ForceCalcInputBase
	{
		glm::vec3 volocity;
		ForceCalcInput(const glm::vec3& volocity = glm::vec3(1.0f)) : volocity(to_meters_per_sec(volocity))
		{
		};
	};


	template<>
	struct ForceCalcInput<ForceType::Spring> : ForceCalcInputBase
	{
		glm::vec3 displacementPos;
		ForceCalcInput(const glm::vec3& displacementPos = glm::vec3(1.0f))
			: displacementPos(to_meters(displacementPos))
		{
		};
	};


	template<>
	struct ForceCalcInput<ForceType::Buoyancy> : ForceCalcInputBase
	{
		Measurible submergedVolume;
		ForceCalcInput(Measurible submergedVolume = 1.0_meter2)
			: submergedVolume(submergedVolume)
		{
		};
	};


	template<>
	struct ForceCalcInput<ForceType::Friction> : ForceCalcInputBase
	{
		Measurible normalForce;
		glm::vec3 volocity;
		ForceCalcInput(Measurible normalForce = 1.0_newtons, const glm::vec3& volocity = glm::vec3(1.0f))
			: normalForce(normalForce), volocity(to_meters_per_sec(volocity))
		{
		};
	};


	template<>
	struct ForceCalcInput<ForceType::Normal> : ForceCalcInputBase
	{
		Measurible mass;
		Measurible angle;
		ForceCalcInput(Measurible mass = 1.0_kilograms, Measurible angleDegree = 0.0_degrees)
			: mass(mass), angle(angleDegree)
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
	using ForceCalcInputBuoyancy = physics::ForceCalcInput<physics::ForceType::Buoyancy>;

}