#pragma once

#include "config.h"


namespace tools
{
	template<glType::ComponentType T>
	class ComponentRegistry
	{
	public:
		ComponentRegistry() = default;

		ComponentRegistry(const ComponentRegistry&) = delete;
		ComponentRegistry& operator=(const ComponentRegistry&) = delete;

		ComponentRegistry(ComponentRegistry&&) noexcept = default;
		ComponentRegistry& operator=(ComponentRegistry&&) noexcept = default;
		
		void invite(const T& component);

		void add_component(glType::Entity entity, const T& component);

		T& get_component(glType::Entity entity);

		const T& get_component(glType::Entity entity) const;
		
		const T& get_or_add_component(glType::Entity entity, const T& component) const;

		void remove_component(glType::Entity entity);

	private:
		std::unordered_map<glType::Entity, T> _components;
		//std::vector<T> _storage; // Alternative if you want to use indices instead of entity IDs
		//std::unordered_map<glType::Entity, size_t> _entityToIndexMap; // If using vector, map entity to index
		//std::unordered_map<size_t, glType::Entity> _indexToEntityMap; // If using vector, map index to entity
	};

}

#include "tools/include/component_registry.inl"
