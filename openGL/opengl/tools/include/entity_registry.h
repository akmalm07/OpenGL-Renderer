#pragma once

#include "config.h"


namespace tools
{

	class EntityRegistry
	{
	public:
		EntityRegistry() = default;

		EntityRegistry(const EntityRegistry&) = delete;
		EntityRegistry& operator=(const EntityRegistry&) = delete;

		EntityRegistry(EntityRegistry&&) = default;
		EntityRegistry& operator=(EntityRegistry&&) = default;

		EntityRegistry& get();

		size_t register_entity(const std::string& entityName);
		
		void unregister_entity(size_t entityId);

		void unregister_entity(const std::string& entityName);

		std::string get_entity_name(int entityId) const;

		std::vector <glType::Entity> get_entities() const;

		glType::Entity get_entity(const std::string& entityName) const;

		bool is_entity_registered(size_t entityId) const;

		bool is_entity_registered(const std::string& entityName) const;

		~EntityRegistry() = default;

	private:
		
		std::unordered_map<glType::Entity, std::string> _entities;
		std::vector <glType::Entity> _entitiesVec; 

		size_t _nextEntityId = 0;
	};

}
