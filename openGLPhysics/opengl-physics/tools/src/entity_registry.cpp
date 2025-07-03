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
		_nextEntityId += 1;

		if (_nextEntityId >= MAX_NUM_OF_ENTITIES)
		{
			throw std::runtime_error("Maximum number of entities reached");
		}

		_entities.emplace(_nextEntityId, entityName);
		_entitiesVec.push_back(_nextEntityId); // Keep track of entity IDs in a vector for potential future use

		return _nextEntityId;
	}

	void EntityRegistry::unregister_entity(size_t entityId)

	{
		_entities.erase(entityId);

		_entitiesVec.erase(std::remove(_entitiesVec.begin(), _entitiesVec.end(), entityId), _entitiesVec.end());

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

	std::vector<glType::Entity> EntityRegistry::get_entities() const
	{
		return _entitiesVec;
	}

	glType::Entity EntityRegistry::get_entity(const std::string& entityName) const
	{
		for (const auto& entity : _entities)
		{
			if (entity.second == entityName)
			{
				return entity.first;
			}
		}
		throw std::runtime_error("Entity not found: " + entityName);
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
				_entitiesVec.erase(std::remove(_entitiesVec.begin(), _entitiesVec.end(), it->first), _entitiesVec.end());
				return;
			}
		}
	}

} // namespace tools
