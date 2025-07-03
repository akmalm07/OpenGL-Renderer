#pragma once
#include "tools/include/component_registry.h"
#include "physics/include/physics_body.h"

namespace tools
{

	template<glType::ComponentType T>
	inline void ComponentRegistry<T>::invite(const T& component)
	{
		component.visit(_components);
	}

	template<glType::ComponentType T>
	inline ComponentRegistry<T>& ComponentRegistry<T>::get_instance()
	{
		static ComponentRegistry instance;
		return instance;
	}

	template<glType::ComponentType T>
	inline void ComponentRegistry<T>::add_component(glType::Entity entity, const T& component)
	{
		auto ptr = std::make_unique<T>(component);
		T* raw_ptr = ptr.get();

		_componentsVec.emplace_back(std::move(ptr));
		_components.emplace(entity, raw_ptr);

		raw_ptr->communicate(entity);
	}

	template<glType::ComponentType T>
	inline T& ComponentRegistry<T>::get_component(glType::Entity entity)
	{
		auto it = _components.find(entity);
		if (it != _components.end())
		{
			return *it->second;
		}
		throw std::runtime_error("Component not found for entity");
	}

	template<glType::ComponentType T>
	inline T* ComponentRegistry<T>::get_component_or_null(glType::Entity entity) const
	{
		auto it = _components.find(entity);
		if (it != _components.end())
		{
			return it->second;
		}
		return nullptr;
	}

	template<glType::ComponentType T>
	inline std::vector<std::unique_ptr<T>>& ComponentRegistry<T>::get_entities_vec()
	{
		return _componentsVec;
	}

	template<glType::ComponentType T>
	inline const std::vector<std::unique_ptr<T>>& ComponentRegistry<T>::get_entities_vec() const
	{
		return _componentsVec;
	}

	template<glType::ComponentType T>
	inline std::unordered_map<glType::Entity, T*>& ComponentRegistry<T>::get_entities()
	{
		return _components;
	}

	template<glType::ComponentType T>
	inline const std::unordered_map<glType::Entity, T*>& ComponentRegistry<T>::get_entities() const
	{
		return _components;
	}

	template<glType::ComponentType T>
	inline const T& ComponentRegistry<T>::get_component(glType::Entity entity) const
	{
		auto it = _components.find(entity);
		if (it != _components.end())
		{
			return *it->second;
		}
		throw std::runtime_error("Component not found for entity");
	}

	template<glType::ComponentType T>
	inline const T& ComponentRegistry<T>::get_or_add_component(glType::Entity entity, const T& component) const
	{
		auto it = _components.find(entity);
		if (it != _components.end())
		{
			return *it->second;
		}
		else
		{
			_components.emplace(entity, component);
			return *_components[entity];
		}
	}

	template<glType::ComponentType T>
	inline void ComponentRegistry<T>::remove_component(glType::Entity entity)
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