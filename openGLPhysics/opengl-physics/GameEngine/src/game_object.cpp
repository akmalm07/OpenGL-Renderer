#include "headers.h"
#include "GameEngine/include/game_object.h"

namespace GameEngine
{

	GameObject::GameObject() = default;


	GameObject::GameObject(const GameObjectBundle& bundle, bool debug)
	: _debug(debug)
	{
		init(bundle);
	}

	void GameObject::init(const GameObjectBundle& bundle)
	{
		_name = bundle.name;

		_mesh.init(bundle.meshBundle);

		if (bundle.rigidBundle.has_value())
		{
			if (bundle.isAABB)
			{
				_rigidBody = std::make_unique<physics::Rigid<glType::BoundType::AABB>>(
					bundle.rigidBundle.value(), _debug);
			}
			else
			{
				_rigidBody = std::make_unique<physics::Rigid<glType::BoundType::OBB>>(
					bundle.rigidBundle.value(), _debug);
			}
		}

		if (bundle.moveibleBundle.has_value())
		{
			if (bundle.isKenimatic)
			{
				_moveibleBody = std::make_unique<physics::Moveible<glType::MovementType::KINEMATIC>>(
					bundle.moveibleBundle.value(), _debug);
			}
			else
			{
				_moveibleBody = std::make_unique<physics::Moveible<glType::MovementType::CONSTANT>>(
					bundle.moveibleBundle.value(), _debug);
			}
		}


		if (_debug)
			std::cout << "GameObject initialized: " << _name << std::endl;
	}

	void GameObject::render(const std::vector<GameObject>& objects, float deltaTime)
	{
		if (_rigidBody.has_value())
		{
			for (const auto& object : objects)
			{
				if (object._rigidBody.has_value())
				{
					_rigidBody.value()->update(*object.get_rigid_body());
				}
			}
		}

		if (_moveibleBody.has_value())
		{
			_moveibleBody.value()->update(deltaTime);
			_mesh.render(_moveibleBody.value()->get_model_matrix()); // the Model matrix is being updated and is affecting the other MESH!!! ERROR FOund
		}
		else
		{
			_mesh.render(glm::mat4(1.0f));
		}

	}

	void GameObject::render(const GameObject& object, float deltaTime)
	{
		if (_rigidBody.has_value())
		{
			if (object._rigidBody.has_value())
			{
				_rigidBody.value()->update(*object.get_rigid_body());
			}	
	
		}

		if (_moveibleBody.has_value())
		{
			_moveibleBody.value()->update(deltaTime);
			_mesh.render(_moveibleBody.value()->get_model_matrix()); // the Model matrix is being updated and is affecting the other MESH!!! ERROR FOund
		}
		else
		{
			_mesh.render(glm::mat4(1.0f));
		}
	}

	void GameObject::render_no_collision(float deltaTime)
	{
		if (_moveibleBody.has_value())
		{
			_moveibleBody.value()->update(deltaTime);
			_mesh.render(_moveibleBody.value()->get_model_matrix());
		}
		else
		{
			_mesh.render(glm::mat4(1.0f)); // Update: MAKE THE ORIGIONAL MODEL MATRIX user spicified
		}
	}

	const std::unique_ptr<physics::RigidBody>& GameObject::get_rigid_body() const
	{
		if (_rigidBody)
		{
			return _rigidBody.value();
		}
		return nullptr;
	}

	void GameObject::add_gravity(const glm::vec3& gravity)
	{
		if (_moveibleBody.has_value())
		{
			_moveibleBody.value()->add_gravity(gravity);
		}
	}

}
