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

		void operator+=(const glm::vec3& val)
		{
			min += val;
			max += val;
		}
	};


	class OBB;

	class AABB;

	class SphereBound;

	struct CollisionPoint
	{
		glm::vec3 position;
		glm::vec3 normal;
		float penetrationDepth;

		bool exists = false;

		CollisionPoint() = default;
		CollisionPoint(const glm::vec3& point, const glm::vec3& norm, float depth)
			: position(point), normal(norm), penetrationDepth(depth), exists(true) {
		}
	};


	class BoundTypeBase
	{
	public:
		virtual std::unique_ptr<BoundTypeBase> clone() const = 0;

		virtual glType::BoundType get_bound_type() const = 0;

		virtual MinMax get_aabb_wrap() const = 0;

		virtual float get_volume() const = 0;

		CollisionPoint touching(BoundTypeBase* other) const;

		virtual void move(const glm::vec3& offset) = 0;

		virtual CollisionPoint touching(const SphereBound& other) const = 0;

		virtual CollisionPoint touching(const AABB& other) const = 0;

		virtual CollisionPoint touching(const OBB& other) const = 0;

		virtual ~BoundTypeBase() = default;
	};


	class CollisionChecker
	{
	private:

		static CollisionPoint aabb_check(const AABB& a, const AABB& b);

		static CollisionPoint sphere_check(const SphereBound& a, const SphereBound& b);

		static CollisionPoint sphere_check(const SphereBound& a, const AABB& b);

		static CollisionPoint sphere_check(const SphereBound& a, const OBB& b);


		static CollisionPoint full_sat_check(const OBB& a, const OBB& b);

		static CollisionPoint partial_sat_check(const OBB& a, const AABB& b);



		friend class AABB;
		friend class OBB;
		friend class SphereBound;



	};

	float project_extent_along_axis(const glm::mat4& rotationMat, const glm::vec3& halfExtent, const glm::vec3& axis);

	float project_extent_along_axis(const std::array<glm::vec3, 3>& axes, const glm::vec3& halfExtent, const glm::vec3& axis);



	glm::vec3 find_center_of_mass(std::vector<glType::Vertex> verts, glUtil::FullStride fullStride, glUtil::PosStride posStride);

	glm::vec3 find_center_of_mass(std::vector <glm::vec3> verts);
	
	MinMax get_min_max_from_vertices(const std::vector<glType::Vertex>& verts, glUtil::FullStride fullStride, glUtil::PosStride posStride);


	MinMax get_min_max_of_mesh(const glUtil::Mesh& mesh, glUtil::FullStride fullStride = glUtil::FullStride::STRIDE_6D, glUtil::PosStride posStride = glUtil::PosStride::STRIDE_3D);
}