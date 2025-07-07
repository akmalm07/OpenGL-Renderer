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
	inline void ComponentRegistry<T>::add_component(glType::Entity entity, T&& component)
	{
		auto ptr = std::make_unique<T>(component);
		T* rawPtr = ptr.get();

		_componentsVec.emplace_back(std::move(ptr));
		_components.emplace(entity, rawPtr);

		rawPtr->communicate(entity);
	}

	inline void ComponentRegistry<physics::PhysicsBody>::add_component(glType::Entity entity, physics::PhysicsBody&& component)
	{
		auto ptr = std::make_unique<physics::PhysicsBody>(std::move(component));
		physics::PhysicsBody* rawPtr = ptr.get();
		
		ptr->add_world_force<physics::ForceType::Gravity>(_defaultGravityForce); // Default gravity force

		rawPtr->communicate_impl(entity);

		this->_componentsVec.emplace_back(std::move(ptr));
		this->_components.emplace(entity, rawPtr);


		add_to_physics_manager(rawPtr);
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

	inline void ComponentRegistry<physics::PhysicsBody>::add_to_physics_manager(physics::PhysicsBody* component)
	{
		if (!_physicsManager)
		{
			_physicsManager = std::make_unique<physics::PhysicsManager<NUM_OF_SPATIAL_PARTIONING_ARENAS>>(*this, glm::vec3(-50.0f), glm::vec3(50.0f));
		}
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
			return it->second;
		}
		return nullptr;
	}

	template<glType::ComponentType T>
	inline std::vector<std::unique_ptr<T>>& ComponentRegistryBase<T>::get_entities_vec()
	{
		return _componentsVec;
	}

	template<glType::ComponentType T>
	inline const std::vector<std::unique_ptr<T>>& ComponentRegistryBase<T>::get_entities_vec() const
	{
		return _componentsVec;
	}

	template<glType::ComponentType T>
	inline std::unordered_map<glType::Entity, T*>& ComponentRegistryBase<T>::get_entities()
	{
		return _components;
	}

	template<glType::ComponentType T>
	inline const std::unordered_map<glType::Entity, T*>& ComponentRegistryBase<T>::get_entities() const
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
		auto ptr = std::make_unique<T>(component);
		T* rawPtr = ptr.get();
		_componentsVec.push_back(std::move(ptr));
		_components[entity] = rawPtr;
		return *rawPtr;
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