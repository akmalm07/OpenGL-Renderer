#pragma once

#include "config.h"

#include "physics/include/aabb.h"



namespace physics
{
	class SphereBound : public BoundTypeBase
	{
	public:

		SphereBound() = default;

		SphereBound(const glm::vec3& center, float radius);

		void init(const glm::vec3& center, float radius);

		float get_radius() const;
		
		bool is_touching(const SphereBound& other) const override final;

		bool is_touching(const AABB& other) const override final;

		bool is_touching(const OBB& other) const override final;

	private:
		float _radius{ 1.0f };

		friend class BoundTypeBase;
	};


}

