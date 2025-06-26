#include "tools/include/component_registry.h"

namespace tools
{
	template<glType::ComponentType T>
	inline void ComponentRegistry<T>::invite(const T& component)
	{
		component.visit(_components)
	}

	template<glType::ComponentType T>
	inline void ComponentRegistry<T>::add_component(glType::Entity entity, const T& component)
	{
		_components.emplace(entity, component);
	}

	template<glType::ComponentType T>
	inline T& ComponentRegistry<T>::get_component(glType::Entity entity)
	{
		auto it = _components.find(entity);
		if (it != _components.end())
		{
			return it->second;
		}
		throw std::runtime_error("Component not found for entity");
	}

	template<glType::ComponentType T>
	inline const T& ComponentRegistry<T>::get_component(glType::Entity entity) const
	{
		auto it = _components.find(entity);
		if (it != _components.end())
		{
			return it->second;
		}
		throw std::runtime_error("Component not found for entity");
	}

	template<glType::ComponentType T>
	inline const T& ComponentRegistry<T>::get_or_add_component(glType::Entity entity, const T& component) const
	{
		auto it = _components.find(entity);
		if (it != _components.end())
		{
			return it->second;
		}
		else
		{
			_components.emplace(entity, component);
			return _components[entity];
		}
	}

	template<glType::ComponentType T>
	inline void ComponentRegistry<T>::remove_component(glType::Entity entity)
	{
		auto it = _components.find(entity);
		if (it != _components.end())
		{
			_components.erase(it);
		}
		else
		{
			throw std::runtime_error("Component not found for entity");
		}
	}
	
}