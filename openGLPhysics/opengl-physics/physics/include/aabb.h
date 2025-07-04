#pragma once

#include "config.h"

#include "physics/include/bound_base.h"

#include "tools/include/stride.h"



namespace physics
{


	class AABB : public BoundTypeBase
	{
	public:
		AABB();

		AABB(const glm::vec3& min, const glm::vec3& max);

		void init(const glm::vec3& min, const glm::vec3& max);

		std::unique_ptr<BoundTypeBase> clone() const override;

		glType::BoundType get_bound_type() const override final;

		void move_reletive_to_dist(const glm::vec3& dist);

		void change(const glm::vec3& offset);
	
		void change_x(float offset);
		void change_y(float offset);
		void change_z(float offset);

		void move(const glm::vec3& volocity, float deltaTime);

		void move(const glm::vec3& volocityTimesDeltaTime);

		glm::vec3 get_min() const;
		glm::vec3 get_max() const;

		bool is_touching(const AABB& other) const override final;

		bool is_touching(const OBB& other) const override final;

		bool is_touching(const SphereBound& other) const override final;

		glm::vec3 get_half_extent() const;

		virtual std::array <glm::vec3, 8> get_corners() const;

		~AABB();

	protected:
		glm::vec3 _halfExtent;

		friend class BoundTypeBase;

	};

}

