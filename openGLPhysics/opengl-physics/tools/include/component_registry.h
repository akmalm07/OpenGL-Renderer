#pragma once

#include "config.h"

#include "physics/include/physics_manager.h"

#include "physics/include/physics_body.h"

#include "physics/include/force.h"



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
		
		T& get_component(glType::Entity entity);
		
		T* get_component_or_null(glType::Entity entity) const;

		std::vector<std::unique_ptr<T>>& get_entities_vec();
		
		const std::vector<std::unique_ptr<T>>& get_entities_vec() const;

		std::unordered_map<glType::Entity, T*>& get_entities();

		const std::unordered_map<glType::Entity, T*>& get_entities() const;

		const T& get_component(glType::Entity entity) const;
		
		T& get_or_add_component(glType::Entity entity, const T& component);
		
		void remove_component(glType::Entity entity);

	protected:
		ComponentRegistryBase() = default;

		std::unordered_map<glType::Entity, T*> _components;
		
		std::vector<std::unique_ptr<T>> _componentsVec;

		//std::vector<T> _storage; // Alternative if you want to use indices instead of entity IDs
		//std::unordered_map<glType::Entity, size_t> _entityToIndexMap; // If using vector, map entity to index
		//std::unordered_map<size_t, glType::Entity> _indexToEntityMap; // If using vector, map index to entity
	};


	template<glType::ComponentType T>
	class ComponentRegistry : public ComponentRegistryBase<T>
	{
	public:
		static ComponentRegistry& get_instance();

		void add_component(glType::Entity entity, T&& component) override;
	private:
		ComponentRegistry() = default;

		using ComponentRegistryBase<T>::_components;

		using ComponentRegistryBase<T>::_componentsVec;
	};

	template<>
	class ComponentRegistry<physics::PhysicsBody> : public ComponentRegistryBase<physics::PhysicsBody>
	{
	public:

		static ComponentRegistry& get_instance();

		void add_component(glType::Entity entity, physics::PhysicsBody&& component);

		physics::PhysicsManager<NUM_OF_SPATIAL_PARTIONING_ARENAS>* get_physics_manager();

		void update_physics_manager();

	private:
		ComponentRegistry() = default;


		void add_to_physics_manager(physics::PhysicsBody* component);

	private:

		std::unique_ptr<physics::PhysicsManager<NUM_OF_SPATIAL_PARTIONING_ARENAS>> _physicsManager;

		physics::GlobalForce<physics::ForceType::Gravity> _defaultGravityForce;

		using ComponentRegistryBase<physics::PhysicsBody>::_components;

		using ComponentRegistryBase<physics::PhysicsBody>::_componentsVec;
	};

}
// Ctrl + < for code search


#include "tools/include/component_registry.inl"
