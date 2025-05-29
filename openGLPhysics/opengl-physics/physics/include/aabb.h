#pragma once

#include "config.h"

#include "tools/include/stride.h"



namespace physics
{
	enum class _PRECALC_COORDS
	{
		YES = 0,
		NO,
	};

	struct MinMax
	{
		glm::vec3 min;
		glm::vec3 max;
	};

	template <_PRECALC_COORDS T>
	class _OBB;

	template <_PRECALC_COORDS T>
	class _AABB;

	class _CHECK_COLLISION
	{
	public:
		template <_PRECALC_COORDS T>
		bool is_touching(const _AABB<T>& a, const _AABB<T>& b) const;
		template <_PRECALC_COORDS T>
		bool is_touching(const _OBB<T>& a, const _OBB<T>& b) const;
		template <_PRECALC_COORDS T>
		bool is_touching(const _AABB<T>& a, const _OBB<T>& b) const;


		template <_PRECALC_COORDS T>
		bool sphere_check(const _AABB<T>& a, const _AABB<T>& b) const;

		template <_PRECALC_COORDS T>
		bool sphere_check(const _OBB<T>& a, const _OBB<T>& b) const;

		template <_PRECALC_COORDS T>
		bool sphere_check(const _AABB<T>& a, const _OBB<T>& b) const;


		template <_PRECALC_COORDS T>
		bool full_sat_check(const _OBB<T>& a, const _OBB<T>& b) const;

		template <_PRECALC_COORDS T>
		bool full_sat_check(const _OBB<T>& a, const _AABB<T>& b) const;


		template <_PRECALC_COORDS T>
		bool _AABB_cast_check(const _OBB<T>& a, const _AABB<T>& b) const;

		template <_PRECALC_COORDS T>
		bool _AABB_cast_check(const _OBB<T>& a, const _OBB<T>& b) const;


		template <_PRECALC_COORDS T>
		bool partial_sat_check(const _OBB<T>& a, const _AABB<T>& b) const;

	protected:

		glm::vec3 _min;
		glm::vec3 _max;

		glm::vec3 _center;
		glm::vec3 _halfExtent;
			
	protected:
		float project_extent_along_axis(const glm::mat4& rotationMat, const glm::vec3& halfExtent, const glm::vec3& axis) const;

		float project_extent_along_axis(const std::array<glm::vec3, 3>& axes, const glm::vec3& halfExtent, const glm::vec3& axis) const;

	};

	class _AABB_COMMON_ANCESTOR : public _CHECK_COLLISION
	{
	public:
		_AABB_COMMON_ANCESTOR();

		_AABB_COMMON_ANCESTOR(const glm::vec3& min, const glm::vec3& max);

		void init(const glm::vec3& min, const glm::vec3& max);

		glm::vec3 get_half_extent() const;

		virtual void move_reletive_to_dist(const glm::vec3& dist);
		
		virtual void move_x_reletive_to_dist(float dist);
		virtual void move_y_reletive_to_dist(float dist);
		virtual void move_z_reletive_to_dist(float dist);

		virtual void change(const glm::vec3& offset);
	
		virtual void change_x(float offset);
		virtual void change_y(float offset);
		virtual void change_z(float offset);

		virtual void move(const glm::vec3& volocity, float deltaTime);

		virtual void move(const glm::vec3& volocityTimesDeltaTime);

		glm::vec3 get_center() const;

		glm::vec3 get_min() const;
		glm::vec3 get_max() const;


		MinMax get_min_max() const;

		virtual ~_AABB_COMMON_ANCESTOR();
	};


	template <>
	class _AABB<_PRECALC_COORDS::NO> : virtual public _AABB_COMMON_ANCESTOR
	{
	public:
		_AABB();
		_AABB(const glm::vec3& min, const glm::vec3& max);

		template <_PRECALC_COORDS T>
		bool is_touching(const _AABB<T>& other) const;

		template <_PRECALC_COORDS T>
		bool is_touching(const _OBB<T>& other) const;

		virtual ~_AABB();

	protected:

	};


	template <>
	class _AABB <_PRECALC_COORDS::YES> : virtual public _AABB_COMMON_ANCESTOR
	{
	public:
		_AABB(); 
		_AABB(const glm::vec3& min, const glm::vec3& max);

		template <_PRECALC_COORDS T>
		bool is_touching(const _AABB<T>& other) const;

		template <_PRECALC_COORDS T>
		bool is_touching(const _OBB<T>& other) const;

		virtual std::array <glm::vec3, 8> get_corners() const;
		
		virtual std::array <glType::Vertex, 48> get_vertices(const glm::vec3& color) const;
		virtual std::array <glType::Index, 36> get_indices() const;

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

		virtual ~_AABB();

	protected:
		std::array <glm::vec3, 8> _corners;
	};

	MinMax get_min_max_from_vertices(const std::vector<glType::Vertex>& verts, FullStride fullStride, PosStride posStride);

	//Compiler tricks to avoid code duplication (CTAD) -- ERROR!
	//_AABB() -> _AABB<_PRECALC_COORDS::NO>;

	//_AABB(const glm::vec3&, const glm::vec3&) -> _AABB<_PRECALC_COORDS::NO>;

	using AABB = _AABB<_PRECALC_COORDS::NO>;

	using AABBCoord = _AABB<_PRECALC_COORDS::YES>;
}

#include "physics/include/aabb.inl"