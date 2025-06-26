#pragma once

#include "config.h"

#include "tools/include/keys.h"


namespace tools
{
	enum class PhysBodyFeatures : uint32_t
	{
		None = 0,

		AABB = 1 << 0,
		OBB = 1 << 1,
		Sphere = 1 << 2,

		Gravity = 1 << 3,
		Drag = 1 << 4,
		Volocity = 1 << 5,
		Force = 1 << 6,
		Mass = 1 << 7,

		AABBKenimatic = AABB | Volocity | Force | Mass,
		AABBConstant = AABB | Volocity | Mass,
		OBBKenimatic = OBB | Volocity | Force | Mass,
		OBBConstant = OBB | Volocity | Mass,
		SphereKenimatic = Sphere | Volocity | Force | Mass,
		SphereConstant = Sphere | Volocity | Mass,

		Count = 8
	};

}
