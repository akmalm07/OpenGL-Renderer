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

		OBB(const glm::vec3& min, const glm::vec3& max, const glm::vec3& xyzRotation);

		void init(const glm::vec3& min, const glm::vec3& max, const glm::vec3& xyzRotation);

		std::unique_ptr<BoundTypeBase> clone() const override;

		glm::vec3 get_rotation() const;

		glm::vec3 get_rotation_degree() const;

		glm::mat4 get_rotation_matrix() const;

		glm::vec3 get_local_min() const;

		glm::vec3 get_local_max() const;

		glm::vec3 get_rotated_min() const;

		glm::vec3 get_rotated_max() const;

		glm::vec3 get_aabb_wrap_min() const;

		glm::vec3 get_aabb_wrap_max() const;

		bool is_touching(const AABB& other) const override final;

		bool is_touching(const OBB& other) const override final;

		bool is_touching(const SphereBound& other) const override final;

		void move_reletive_to_dist(const glm::vec3& dist);

		void change(const glm::vec3& offset, const glm::vec3& rotation);

		void change_x(float offset, float rotation);
		void change_y(float offset, float rotation);
		void change_z(float offset, float rotation);

		void move(const glm::vec3& offset);

		void rotate(const glm::vec3& rotation);

		std::array <glm::vec3, 8> get_corners() const;

		virtual ~OBB();

	private:

		glm::vec3 _xyzRotation = glm::vec3(0.0f);
		glm::mat4 _rotationMat = glm::mat4(1.0f);

		glm::vec3 _localMin = glm::vec3(0.0f);
		glm::vec3 _localMax = glm::vec3(0.0f);

		glm::vec3 _rotation = glm::vec3(0.0f);

		glm::vec3 _halfExtent = glm::vec3(0.0f);

		friend class BoundTypeBase;

	private:
		glm::vec3 get_extent() const;
	};



	glm::vec3 project_onto_axis(const glm::vec3& vec);

	glm::vec3 projection(const glm::vec3& v, const glm::vec3& u);

	glm::vec3 project_onto_plane(const glm::vec3& vec, const glm::vec3& plane);

}
