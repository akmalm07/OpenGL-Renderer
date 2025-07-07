#pragma once

#include "config.h"

#include "physics/include/units.h"


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
		float elasticity = 0.5f; 
		glm::vec3 initalForce = glm::vec3(0.0f);

		glm::vec3 centerOfMass = glm::vec3(0.0f); 

		std::function<void(glType::Entity)> collisionFunc;

		PhysicsBodyBundleBase()
		{
			using namespace units;
			centerPosition = units::to_meters(glm::vec3(0.0f));
			volocity = units::to_meters_per_sec(glm::vec3(0.0f));
			mass = 1.0_kilograms;
			gravityAffected = false;
			initalForce = units::to_newtons(glm::vec3(0.0f));
		}

	};

	struct PhysicsBodyBundleAABB : public PhysicsBodyBundleBase
	{
	public:
		glType::BoundType bound_type() const override { return glType::BoundType::AABB; }

		glm::vec3 min;
		glm::vec3 max;

		PhysicsBodyBundleAABB()
		{
			using namespace units;
			min = units::to_meters(glm::vec3(-1.0f));
			max = units::to_meters(glm::vec3(1.0f));
		}
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

		PhysicsBodyBundleSphere()
		{
			using namespace units;
			radius = 1.0_meters;
		}
	};


}