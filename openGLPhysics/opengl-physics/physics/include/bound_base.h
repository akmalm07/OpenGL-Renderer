#pragma once

#include "config.h"

#include "tools/include/stride.h"

namespace glUtil
{
	class Mesh;
}

namespace physics
{
	struct MinMax
	{
		glm::vec3 min;
		glm::vec3 max;
	};


	class OBB;

	class AABB;

	class SphereBound;

	struct TouchingData
	{
		bool result = false;
		std::optional<glm::vec3> collisionPoint;
	};


	class BoundTypeBase
	{
	public:
		virtual std::unique_ptr<BoundTypeBase> clone() const = 0;

		virtual glType::BoundType get_bound_type() const = 0;

		MinMax get_min_max() const;

		void change(const glm::vec3& val);

		void change_x(float offset);
		
		void change_y(float offset);

		void change_z(float offset);

		glm::vec3 get_center() const;

		float get_volume() const;

		TouchingData is_touching(BoundTypeBase* other);

		virtual TouchingData is_touching(const SphereBound& other) const = 0;

		virtual TouchingData is_touching(const AABB& other) const = 0;

		virtual TouchingData is_touching(const OBB& other) const = 0;

		virtual ~BoundTypeBase() = default;

	protected:
		TouchingData is_touching(const AABB& a, const AABB& b) const;

		TouchingData is_touching(const OBB& a, const OBB& b) const;

		TouchingData is_touching(const AABB& a, const OBB& b) const;



		bool sphere_check(const AABB& a, const AABB& b) const;

		bool sphere_check(const OBB& a, const OBB& b) const;

		bool sphere_check(const AABB& a, const OBB& b) const;


		TouchingData sphere_check(const SphereBound& a, const SphereBound& b) const;

		TouchingData sphere_check(const SphereBound& a, const AABB& b) const;

		TouchingData sphere_check(const SphereBound& a, const OBB& b) const;



		TouchingData full_sat_check(const OBB& a, const OBB& b) const;

		TouchingData full_sat_check(const OBB& a, const AABB& b) const;



		TouchingData aabb_cast_check(const OBB& a, const AABB& b) const;

		TouchingData aabb_cast_check(const OBB& a, const OBB& b) const;


		TouchingData partial_sat_check(const OBB& a, const AABB& b) const;

		float project_extent_along_axis(const glm::mat4& rotationMat, const glm::vec3& halfExtent, const glm::vec3& axis) const;

		float project_extent_along_axis(const std::array<glm::vec3, 3>& axes, const glm::vec3& halfExtent, const glm::vec3& axis) const;

	protected:

		glm::vec3 _center;

		glm::vec3 _min;
		glm::vec3 _max;
	};


	glm::vec3 find_center_of_mass(std::vector<glType::Vertex> verts, glUtil::FullStride fullStride, glUtil::PosStride posStride);

	glm::vec3 find_center_of_mass(std::vector <glm::vec3> verts);
	
	MinMax get_min_max_from_vertices(const std::vector<glType::Vertex>& verts, glUtil::FullStride fullStride, glUtil::PosStride posStride);


	MinMax get_min_max_of_mesh(const glUtil::Mesh& mesh, glUtil::FullStride fullStride = glUtil::FullStride::STRIDE_6D, glUtil::PosStride posStride = glUtil::PosStride::STRIDE_3D);
}