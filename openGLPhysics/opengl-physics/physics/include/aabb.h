#pragma once

#include "config.h"

#include "tools/include/stride.h"



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


	class BoundTypeBase
	{
	public:



		MinMax get_min_max() const;

		glm::vec3 get_center() const;

		float get_volume() const;

		virtual bool is_touching(const SphereBound& other) const = 0;

		virtual bool is_touching(const AABB& other) const = 0;

		virtual bool is_touching(const OBB& other) const = 0;


	protected:
		bool is_touching(const AABB& a, const AABB& b) const;
		
		bool is_touching(const OBB& a, const OBB& b) const;
		
		bool is_touching(const AABB& a, const OBB& b) const;


		
		bool sphere_check(const AABB& a, const AABB& b) const;

		bool sphere_check(const OBB& a, const OBB& b) const;
		
		bool sphere_check(const AABB& a, const OBB& b) const;
		
		
		bool sphere_check(const SphereBound& a, const SphereBound& b) const;

		bool sphere_check(const SphereBound& a, const AABB& b) const;

		bool sphere_check(const SphereBound& a, const OBB& b) const;


		
		bool full_sat_check(const OBB& a, const OBB& b) const;
		
		bool full_sat_check(const OBB& a, const AABB& b) const;


		
		bool aabb_cast_check(const OBB& a, const AABB& b) const;
		
		bool aabb_cast_check(const OBB& a, const OBB& b) const;


		bool partial_sat_check(const OBB& a, const AABB& b) const;

		float project_extent_along_axis(const glm::mat4& rotationMat, const glm::vec3& halfExtent, const glm::vec3& axis) const;

		float project_extent_along_axis(const std::array<glm::vec3, 3>& axes, const glm::vec3& halfExtent, const glm::vec3& axis) const;

	protected:

		glm::vec3 _center;

		glm::vec3 _min;
		glm::vec3 _max;
	};


	class HasMinMax
	{
		//Finish from here to alow the class partioning reqister to know its bound types so that it can find the positions of the item
	};

	class AABB : public BoundTypeBase
	{
	public:
		AABB();

		AABB(const glm::vec3& min, const glm::vec3& max);

		void init(const glm::vec3& min, const glm::vec3& max);

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

		MinMax get_min_max() const;

		~AABB();

	protected:
		glm::vec3 _halfExtent;

		friend class BoundTypeBase;

	};

	/*

	template <>
	class aabb <_PRECALC_COORDS::YES> : public AABB
	{
	public:
		aabb(); 
		aabb(const glm::vec3& min, const glm::vec3& max);

		
		bool is_touching(const AABB& other) const;

		
		bool is_touching(const OBB& other) const;

		std::array <glm::vec3, 8> get_corners() const;
		
		std::array <glType::Vertex, 48> get_vertices(const glm::vec3& color) const;
		std::array <glType::Index, 36> get_indices() const;

		void move_reletive_to_dist(const glm::vec3& dist) override;

		void move_x_reletive_to_dist(float dist) override;
		void move_y_reletive_to_dist(float dist) override;
		void move_z_reletive_to_dist(float dist) override;

		void move(const glm::vec3& volocity, float deltaTime) override;

		void move(const glm::vec3& volocityTimesDeltaTime) override;

		void change(const glm::vec3& offset) override;

		void change_x(float offset) override;
		void change_y(float offset) override;
		void change_z(float offset) override;

		~aabb();

	protected:
		std::array <glm::vec3, 8> _corners;
	};

	MinMax get_min_max_from_vertices(const std::vector<glType::Vertex>& verts, FullStride fullStride, PosStride posStride);

	//Compiler tricks to avoid code duplication (CTAD) -- ERROR!
	//aabb() -> aabb<_PRECALC_COORDS::NO>;

	//aabb(const glm::vec3&, const glm::vec3&) -> aabb<_PRECALC_COORDS::NO>;

	using AABB = aabb<_PRECALC_COORDS::NO>;

	using AABBCoord = aabb<_PRECALC_COORDS::YES>;
	*/
}

