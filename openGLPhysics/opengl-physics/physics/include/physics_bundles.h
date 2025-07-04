#pragma once

#include "config.h"

#include "physics/include/force.h"


namespace physics
{
    struct PhysicsBodyBundleBase
    {
    public:

		virtual glType::BoundType bound_type() const = 0;

		glm::vec3 centerPosition = glm::vec3(0.0f);
		glm::vec3 volocity = glm::vec3(0.0f);
		float mass = 1.0f; 
		bool gravityAffected = false;
		glm::vec3 initalForce = glm::vec3(0.0f);

	};

	struct PhysicsBodyBundleAABB : public PhysicsBodyBundleBase
	{
	public:
		glType::BoundType bound_type() const override { return glType::BoundType::AABB; }

		glm::vec3 min;
		glm::vec3 max;
	};

	struct PhysicsBodyBundleOBB : public PhysicsBodyBundleAABB
	{
	public:
		glType::BoundType bound_type() const override { return glType::BoundType::OBB; }
		glm::vec3 rotation = glm::vec3(0.0f); 
	};

	struct PhysicsBodyBundleSphere : public PhysicsBodyBundleAABB
	{
	public:
		glType::BoundType bound_type() const override { return glType::BoundType::Sphere; }
		float radius = 1.0f;
	};


}