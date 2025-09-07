#pragma once

#include "config.h"

#include "glUtil/include/mesh.h"

#include "physics/include/bound_base.h"



namespace physics
{

	enum class Axis
	{
		X = 0,
		Y,
		Z
	};


	class OBB : public BoundTypeBase 
	{
	public:
		OBB();

		OBB(const glm::vec3& min, const glm::vec3& max, const glm::vec3& degrees);

		OBB(const glm::vec3& center, float length, float width, float height, const glm::vec3& degrees);

		std::unique_ptr<BoundTypeBase> clone() const override final;

		MinMax get_aabb_wrap() const override final;

		float get_volume() const override final;

		glType::BoundType get_bound_type() const override final;

		glm::mat3 get_rotation() const;

		MinMax get_min_max() const;

		MinMax get_aabb_wrap_min_max() const;

		CollisionPoint touching(const AABB& other) const override final;

		CollisionPoint touching(const OBB& other) const override final;

		CollisionPoint touching(const SphereBound& other) const override final;

		void move(const glm::vec3& offset) override final;

		void full_move(const glm::vec3& offset, const glm::vec3& rotation);

		void set_pos(const glm::vec3& pos);

		void rotate(const glm::vec3& deg);

		virtual ~OBB();


	private:

		glm::vec3 _center;

		glm::vec3 _rotationDeg;

		glm::vec3 _halfExtent = glm::vec3(1.0f);

		glm::quat _orientation;

		friend class CollisionChecker;

	};



	glm::vec3 project_onto_axis(const glm::vec3& vec, Axis axis);

	glm::vec3 projection(const glm::vec3& v, const glm::vec3& u);

	glm::vec3 project_onto_plane(const glm::vec3& vec, const glm::vec3& plane);

}
