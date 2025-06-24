#pragma once

#include "config.h"


namespace tools
{

	class EntityRegistry
	{
	public:

		EntityRegistry(const EntityRegistry&) = delete;
		EntityRegistry& operator=(const EntityRegistry&) = delete;

		EntityRegistry& get();

		size_t register_entity(const std::string& entityName);
		
		void unregister_entity(size_t entityId);

		void unregister_entity(const std::string& entityName);

		std::string get_entity_name(int entityId) const;

		bool is_entity_registered(size_t entityId) const;

		bool is_entity_registered(const std::string& entityName) const;

	private:
		EntityRegistry() = default;

		std::unordered_map<glType::Entity, std::string> _entities;
		size_t _nextEntityId = 0;
	};

}
