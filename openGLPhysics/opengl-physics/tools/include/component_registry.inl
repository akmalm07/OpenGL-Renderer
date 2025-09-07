#pragma once

#include "tools/include/component_registry.h"// Certualr Dependency


namespace tools
{

	template<glType::ComponentType T>
	inline ComponentRegistry<T>& ComponentRegistry<T>::get_instance()
	{
		static ComponentRegistry instance;
		return instance;
	}


	inline ComponentRegistry<physics::PhysicsBody>& ComponentRegistry<physics::PhysicsBody>::get_instance()
	{
		static ComponentRegistry instance;
		return instance;
	}



	template<glType::ComponentType T>
	inline T& ComponentRegistry<T>::add_and_get_component(glType::Entity entity, T&& component)
	{
		return *add_and_get_comp_shared(entity, std::forward<T>(component));
	}

	template<glType::ComponentType T>
	inline std::shared_ptr<T> ComponentRegistry<T>::add_and_get_component_shared(glType::Entity entity, T&& component)
	{
		return add_and_get_comp_shared(entity, std::forward<T>(component));
	}

	template<glType::ComponentType T>
	inline void ComponentRegistry<T>::add_component(glType::Entity entity, T&& component)
	{
		auto ptr = std::make_shared<T>(component); //TODO: Add Allocator
		_componentsVec.emplace_back(ptr);
		_components.emplace(entity, ptr);
		ptr->communicate(entity);
	}

	template<glType::ComponentType T>
	inline std::shared_ptr<T> ComponentRegistry<T>::add_and_get_comp_shared(glType::Entity entity, T&& component)
	{
		auto ptr = std::make_shared<T>(component); //TODO: Add Allocator

		_componentsVec.emplace_back(ptr);
		_components.emplace(entity, ptr);

		ptr->communicate(entity);

		return ptr;
	}

	inline void ComponentRegistry<physics::PhysicsBody>::add_component(glType::Entity entity, physics::PhysicsBody&& component)
	{
		auto ptr = std::make_shared<physics::PhysicsBody>(component); //TODO: Add Allocator

		_componentsVec.emplace_back(ptr);
		_components.emplace(entity, ptr);

		ptr->add_world_force<physics::ForceType::Gravity>(_defaultGravityForce);

		ptr->communicate_impl(entity);

		add_to_physics_manager(ptr);
	}

	inline std::shared_ptr<physics::PhysicsBody> ComponentRegistry<physics::PhysicsBody>::add_and_get_component_shared(glType::Entity entity, physics::PhysicsBody&& component)
	{
		return add_and_get_comp_shared(entity, std::forward<physics::PhysicsBody>(component));
	}

	inline physics::PhysicsBody& ComponentRegistry<physics::PhysicsBody>::add_and_get_component(glType::Entity entity, physics::PhysicsBody&& component)
	{

		return *add_and_get_comp_shared(entity, std::forward<physics::PhysicsBody>(component));
	}

	inline physics::PhysicsManager<NUM_OF_SPATIAL_PARTIONING_ARENAS>* ComponentRegistry<physics::PhysicsBody>::get_physics_manager()
	{
		return _physicsManager.get();
	}

	inline void ComponentRegistry<physics::PhysicsBody>::update_physics_manager()
	{
		if (_physicsManager)
		{
			_physicsManager->update();
		}
	}

	inline std::shared_ptr<physics::PhysicsBody> ComponentRegistry<physics::PhysicsBody>::add_and_get_comp_shared(glType::Entity entity, physics::PhysicsBody&& component)
	{
		auto ptr = std::make_shared<physics::PhysicsBody>(component); //TODO: Add Allocator

		_componentsVec.emplace_back(ptr);
		_components.emplace(entity, ptr);

		ptr->add_world_force<physics::ForceType::Gravity>(_defaultGravityForce);

		ptr->communicate_impl(entity);

		add_to_physics_manager(ptr);
	
		return ptr;
	}

	inline void ComponentRegistry<physics::PhysicsBody>::add_to_physics_manager(const std::shared_ptr<physics::PhysicsBody>& component)
	{
		if (!_physicsManager)
		{
			_physicsManager = std::make_unique<physics::PhysicsManager<NUM_OF_SPATIAL_PARTIONING_ARENAS>>(*this, glm::vec3(-10.0f), glm::vec3(10.0f));
		}
		if (component)
			_physicsManager->register_body(*component);
	}


	template<glType::ComponentType T>
	inline T& ComponentRegistryBase<T>::get_component(glType::Entity entity)
	{
		auto it = _components.find(entity);
		if (it != _components.end())
		{
			return *it->second;
		}
		throw std::runtime_error("Component not found for entity");
	}

	template<glType::ComponentType T>
	inline T* ComponentRegistryBase<T>::get_component_or_null(glType::Entity entity) const
	{
		auto it = _components.find(entity);
		if (it != _components.end())
		{
			return it->second.get();
		}
		return nullptr;
	}

	template<glType::ComponentType T>
	inline std::vector<std::shared_ptr<T>>& ComponentRegistryBase<T>::get_entities_vec()
	{
		return _componentsVec;
	}

	template<glType::ComponentType T>
	inline const std::vector<std::shared_ptr<T>>& ComponentRegistryBase<T>::get_entities_vec() const
	{
		return _componentsVec;
	}

	template<glType::ComponentType T>
	inline std::unordered_map<glType::Entity, std::shared_ptr<T>>& ComponentRegistryBase<T>::get_entities()
	{
		return _components;
	}

	template<glType::ComponentType T>
	inline const std::unordered_map<glType::Entity, std::shared_ptr<T>>& ComponentRegistryBase<T>::get_entities() const
	{
		return _components;
	}

	template<glType::ComponentType T>
	inline const T& ComponentRegistryBase<T>::get_component(glType::Entity entity) const
	{
		auto it = _components.find(entity);
		if (it != _components.end())
		{
			return *it->second;
		}
		throw std::runtime_error("Component not found for entity");
	}

	template<glType::ComponentType T>
	inline T& ComponentRegistryBase<T>::get_or_add_component(glType::Entity entity, const T& component)
	{
		auto it = _components.find(entity);
		if (it != _components.end())
		{
			return add_and_get_component(entity, component);
			
		}
		return *it;
	}

	template<glType::ComponentType T>
	inline void ComponentRegistryBase<T>::remove_component(glType::Entity entity)
	{
		auto it = _components.find(entity);
		if (it != _components.end())
		{
			_components.erase(it);
			_componentsVec.erase(std::remove_if(_componentsVec.begin(), _componentsVec.end(),
				[entity](const T& comp) { return comp.entity_id() == entity; }), _componentsVec.end());
		}
		else
		{
			throw std::runtime_error("Component not found for entity");
		}
	}
}