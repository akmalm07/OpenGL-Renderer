#pragma once

#include "config.h"

#include "physics/include/bound_base.h"



namespace physics
{
	class SphereBound : public BoundTypeBase
	{
	public:

		SphereBound() = default;

		SphereBound(const glm::vec3& center, float radius);
		
		SphereBound(const glm::vec3& min,const glm::vec3& max, float radius);

		std::unique_ptr<BoundTypeBase> clone() const override;

		MinMax get_aabb_wrap() const override final;

		float get_volume() const override final;

		glType::BoundType get_bound_type() const override final;

		glm::vec3 get_center() const;

		void move(const glm::vec3& offset) override final;

		float get_radius() const;
		
		CollisionPoint touching(const SphereBound& other) const override final;

		CollisionPoint touching(const AABB& other) const override final;

		CollisionPoint touching(const OBB& other) const override final;

	private:
		float _radius = 1.0f;
		glm::vec3 _center = glm::vec3(0.0f);

		friend class CollisionChecker;
	};


}

