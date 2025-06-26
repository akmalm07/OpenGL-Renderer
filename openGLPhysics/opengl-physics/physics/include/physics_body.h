#pragma once

#include "config.h"

#include "physics/include/aabb.h"

#include "physics/include/obb.h"

#include "physics/include/acceleration.h"

#include "physics/include/volocity.h"

#include "physics/include/force.h"

#include "tools/include/features.h"

#include "physics/include/units.h"

namespace physics
{
	struct MassComponent
	{
	public:
		Measurible mass; 
	};

	struct AABBComponent
	{
	public:
		AABB aabb; // Axis-Aligned Bounding Box
	};

	struct VelocityComponent
	{
	public:
		Volocity volocity; // Represents the velocity of the body
	};

	struct ForceComponent
	{
	public:
		Acceleration force; // Represents the force acting on the body
	};

	struct Empty
	{
	};

	template<tools::PhysBodyFeatures Features>
	class PhysicsBody : 
		public glType::Component<PhysicsBody<Features>>,
		
		public std::conditional_t<has(Features, tools::PhysBodyFeatures::AABB), AABBComponent, Empty>,
		public std::conditional_t<has(Features, tools::PhysBodyFeatures::Volocity), VelocityComponent, Empty>,
		public std::conditional_t<has(Features, tools::PhysBodyFeatures::Force), ForceComponent, Empty>,
		public std::conditional_t<has(Features, tools::PhysBodyFeatures::Mass), MassComponent, Empty>
	{
	};
}

