#pragma once

#include "config.h"

#include "glUtil/include/mesh.h"

#include "physics/include/rigid.h"

#include "physics/include/moveible.h"

#include "GameEngine/include/game_obj_bundle.h"

namespace GameEngine
{
	class GameObject
	{
	public:

		GameObject();

		GameObject(const GameObjectBundle& bundle, bool debug = false);

		void init(const GameObjectBundle& bundle);

		void render(const std::vector<GameObject>& objects, float deltaTime);
		
		void render(const GameObject& object, float deltaTime);
		
		void render_no_collision(float deltaTime);

		const std::unique_ptr<physics::RigidBody>& get_rigid_body() const;

		void add_gravity(const glm::vec3& gravity);

	protected:

		glUtil::Mesh _mesh;

		std::optional<std::unique_ptr<physics::RigidBody>> _rigidBody;

		std::optional<std::unique_ptr<physics::MoveibleBody>> _moveibleBody;

		std::string_view _name;

		bool _debug = false;
	};

	/*
	template<glType::MovementType T, glType::BoundType T>
	class GameObjectT
	{
	public:

		GameObjectT();

		GameObjectT(const GameObjectBundle& bundle, bool debug = false);

		void init(const GameObjectBundle& bundle);

		void render(const std::vector<GameObjectT>& objects, float deltaTime);

		const std::unique_ptr<physics::RigidBody>& get_rigid_body() const;

		void add_gravity(const glm::vec3& gravity);

	protected:

		glUtil::Mesh _mesh;

		std::string_view _name;

		bool _debug = false;
	};
	*/
}