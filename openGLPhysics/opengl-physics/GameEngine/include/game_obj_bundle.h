#pragma once


#include "config.h"

#include "glUtil/include/mesh_bundle.h"

#include "glUtil/include/texture.h"

#include "physics/include/physics_bundles.h"

namespace GameEngine
{

	struct GameObjectBundle
	{
		std::string_view name;
		
		glUtil::MeshBundle meshBundle;

		std::optional<physics::RigidBundle> rigidBundle = std::nullopt;
		bool isAABB = true;

		std::optional<physics::MoveibleBundle> moveibleBundle = std::nullopt;
		bool isKenimatic = true;

		glm::vec3 translation = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);

	};
}