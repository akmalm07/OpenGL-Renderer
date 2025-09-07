#pragma once

#include "config.h"

#include "physics/include/bound_base.h"

#include "tools/include/stride.h"



namespace physics
{


	class AABB : public BoundTypeBase // Axis Aligned Bounding Box (Non rotatible)
	{
	public:
		AABB() = default;

		AABB(const glm::vec3& min, const glm::vec3& max);

		std::unique_ptr<BoundTypeBase> clone() const override;

		glType::BoundType get_bound_type() const override final;

		MinMax get_aabb_wrap() const override final;

		float get_volume() const override final;

		glm::vec3 get_center() const;

		MinMax get_min_max() const;

		void move(const glm::vec3& offset) override final;

		CollisionPoint touching(const AABB& other) const override final;

		CollisionPoint touching(const OBB& other) const override final;

		CollisionPoint touching(const SphereBound& other) const override final;

		glm::vec3 get_half_extent() const;

		~AABB();

	protected:

		friend class CollisionChecker;


		MinMax _minMax;

		friend class BoundTypeBase;

	};

}

