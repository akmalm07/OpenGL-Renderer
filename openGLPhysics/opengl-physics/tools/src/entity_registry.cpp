#include "headers.h"
#include "tools/include/entity_registry.h"


namespace tools
{
	// Singleton instance
	EntityRegistry& EntityRegistry::get()
	{
		static EntityRegistry instance;
		return instance;
	}
	size_t EntityRegistry::register_entity(const std::string& entityName)

	{
		if (_nextEntityId >= MAX_NUM_OF_ENTITIES)
		{
			throw std::runtime_error("Maximum number of entities reached");
		}

		_entities.emplace(_nextEntityId, entityName);

		return _nextEntityId++;
	}

	void EntityRegistry::unregister_entity(size_t entityId)

	{
		_entities.erase(entityId);
	}
	
	std::string EntityRegistry::get_entity_name(int entityId) const
	{
		auto it = _entities.find(entityId);
		if (it != _entities.end())
		{
			return it->second;
		}
		return "Unknown Entity";
	}

	bool EntityRegistry::is_entity_registered(size_t entityId) const

	{
		return _entities.find(entityId) != _entities.end();
	}

	bool EntityRegistry::is_entity_registered(const std::string& entityName) const

	{
		for (const auto& entity : _entities)
		{
			if (entity.second == entityName)
			{
				return true;
			}
		}
		return false;
	}

	void EntityRegistry::unregister_entity(const std::string& entityName)
	{
		for (auto it = _entities.begin(); it != _entities.end(); ++it)
		{
			if (it->second == entityName)
			{
				_entities.erase(it);
				return;
			}
		}
	}

	EntityRegistry::~EntityRegistry()
	{
		_entities.clear();
		_nextEntityId = 0;
	}
} // namespace tools
