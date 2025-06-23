#pragma once

#include "config.h"

#include "glUtil/include/mesh.h"

#include "physics/include/aabb.h"



namespace physics
{

	enum class Axis
	{
		X = 0,
		Y,
		Z
	};


	class OBB : public AABB
	{
	public:
		OBB();

		OBB(const glm::vec3& min, const glm::vec3& max, const glm::vec3& xyzRotation, float rotationDegree);

		void init(const glm::vec3& min, const glm::vec3& max, const glm::vec3& xyzRotation, float rotationDegree);

		glm::vec3 get_rotation() const;

		float get_rotation_degree() const;

		glm::mat4 get_rotation_matrix() const;

		glm::vec3 get_local_min() const;

		glm::vec3 get_local_max() const;

		glm::vec3 get_aabb_wrap_min() const;

		glm::vec3 get_aabb_wrap_max() const;

		std::array <glm::vec3, 8> get_corners() const;

		virtual ~OBB();

	private:

		float _rotationDeg = 0.0f;
		glm::vec3 _xyzRotation = glm::vec3(0.0f);
		glm::mat4 _rotationMat = glm::mat4(1.0f);

		glm::vec3 _localMin = glm::vec3(0.0f);
		glm::vec3 _localMax = glm::vec3(0.0f);

		glm::vec3 _halfExtent = glm::vec3(0.0f);

		friend class BoundTypeBase;

	private:
		glm::vec3 get_extent() const;
	};

	/*
	template<>
	class _OBB<_PRECALC_COORDS::NO> : public _AABB<_PRECALC_COORDS::NO>, public OBB
	{
	public:
		
		_OBB();
		_OBB(const glm::vec3& min, const glm::vec3& max, const glm::vec3& xyzRotation, float rotationDegree);

		~_OBB();
	};
	

	template<> // To be TESTED ...
	class _OBB<_PRECALC_COORDS::YES> : public _AABB<_PRECALC_COORDS::YES>, public OBB
	{
	public:
		_OBB();
		_OBB(const glm::vec3& min, const glm::vec3& max, const glm::vec3& xyzRotation, float rotationDegree);

		~_OBB();
	};
	*/


	glm::vec3 project_onto_axis(const glm::vec3& vec);

	glm::vec3 projection(const glm::vec3& v, const glm::vec3& u);

	glm::vec3 project_onto_plane(const glm::vec3& vec, const glm::vec3& plane);

}
