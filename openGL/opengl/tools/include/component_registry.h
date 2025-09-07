#pragma once

#include "config.h"



namespace tools
{	
	template<glType::ComponentType T>
	class ComponentRegistryBase
	{
	public:

		ComponentRegistryBase(const ComponentRegistryBase&) = delete;
		ComponentRegistryBase& operator=(const ComponentRegistryBase&) = delete;

		ComponentRegistryBase(ComponentRegistryBase&&) noexcept = delete;
		ComponentRegistryBase& operator=(ComponentRegistryBase&&) noexcept = delete;
			
		virtual void add_component(glType::Entity entity, T&& component) = 0;

		virtual std::shared_ptr<T> add_and_get_component_shared(glType::Entity entity, T&& component) = 0;

		virtual T& add_and_get_component(glType::Entity entity, T&& component) = 0;

		T& get_component(glType::Entity entity);
		
		T* get_component_or_null(glType::Entity entity) const;

		std::vector<std::shared_ptr<T>>& get_entities_vec();
		
		const std::vector<std::shared_ptr<T>>& get_entities_vec() const;

		std::unordered_map<glType::Entity, std::shared_ptr<T>>& get_entities();

		const std::unordered_map<glType::Entity, std::shared_ptr<T>>& get_entities() const;

		const T& get_component(glType::Entity entity) const;
		
		T& get_or_add_component(glType::Entity entity, const T& component);
		
		void remove_component(glType::Entity entity);

	protected:
		ComponentRegistryBase() = default;

		std::unordered_map<glType::Entity, std::shared_ptr<T>> _components;
		
		std::vector<std::shared_ptr<T>> _componentsVec;

		//std::vector<T> _storage; // Alternative if you want to use indices instead of entity IDs
		//std::unordered_map<glType::Entity, size_t> _entityToIndexMap; // If using vector, map entity to index
		//std::unordered_map<size_t, glType::Entity> _indexToEntityMap; // If using vector, map index to entity
	};


	template<glType::ComponentType T>
	class ComponentRegistry : public ComponentRegistryBase<T>
	{
	public:
		static ComponentRegistry& get_instance();

		T& add_and_get_component(glType::Entity entity, T&& component) override;

		std::shared_ptr<T> add_and_get_component_shared(glType::Entity entity, T&& component) override;
		
		void add_component(glType::Entity entity, T&& component) override;

	private:
		
		ComponentRegistry() = default;

		std::shared_ptr<T> add_and_get_comp_shared(glType::Entity entity, T&& component);


		using ComponentRegistryBase<T>::_components;

		using ComponentRegistryBase<T>::_componentsVec;
	};


}
// Ctrl + < for code search


#include "tools/include/component_registry.inl"
