#pragma once

#include "config.h"


namespace tools
{	
	template<glType::ComponentType T>
	class ComponentRegistry
	{
	public:

		ComponentRegistry(const ComponentRegistry&) = delete;
		ComponentRegistry& operator=(const ComponentRegistry&) = delete;

		ComponentRegistry(ComponentRegistry&&) noexcept = delete;
		ComponentRegistry& operator=(ComponentRegistry&&) noexcept = delete;
	
		static ComponentRegistry& get_instance();
		
		void add_component(glType::Entity entity, T&& component); // ERR!
		
		T& get_component(glType::Entity entity);
		
		T* get_component_or_null(glType::Entity entity) const;

		std::vector<std::unique_ptr<T>>& get_entities_vec();
		
		const std::vector<std::unique_ptr<T>>& get_entities_vec() const;

		std::unordered_map<glType::Entity, T*>& get_entities();

		const std::unordered_map<glType::Entity, T*>& get_entities() const;

		const T& get_component(glType::Entity entity) const;
		
		const T& get_or_add_component(glType::Entity entity, const T& component) const;
		
		void remove_component(glType::Entity entity);

	private:
		ComponentRegistry() = default;

		std::unordered_map<glType::Entity, T*> _components;
		
		std::vector<std::unique_ptr<T>> _componentsVec;



		void invite(const T& component);

		//std::vector<T> _storage; // Alternative if you want to use indices instead of entity IDs
		//std::unordered_map<glType::Entity, size_t> _entityToIndexMap; // If using vector, map entity to index
		//std::unordered_map<size_t, glType::Entity> _indexToEntityMap; // If using vector, map index to entity
	};
}

#include "tools/include/component_registry.inl"
