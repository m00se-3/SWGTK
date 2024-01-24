#include "Entity.hpp"

namespace cts
{
	Registry::CreateResult Registry::Create()
	{
		const auto size = _entities.size();
		
		if (size < _entities.capacity())
		{
			auto entity = _entities.emplace_back(size);
			_compMap.insert_or_assign(entity, std::unordered_map<std::string, uint32_t>{});
			return entity;
		}
		else if(!_freeEntities.empty())
		{
			auto id = _freeEntities.front();
			_freeEntities.pop_front();
			return id;
		}
		else
		{
			return EntityError::no_storage;
		}
	}

	void Registry::SetComponentCapacity(const std::string& name, uint32_t size)
	{
		if (_compContainer.contains(name))
		{
			_compContainer.at(name).reserve(size);
		}
	}

	void Registry::SetEntityCapacity(uint32_t size)
	{
		_entities.reserve(size);
	}

	void Registry::Destroy(Entity entity)
	{
		size_t location = 0u;

		for (auto ent : _entities)
		{
			if (ent == entity)
			{
				_freeEntities.emplace_back(location);
				
				for (auto& [name, id] : _compMap.at(ent))
				{
					_freeLists.at(name).emplace_back(id);
					_compContainer.at(name)[id] = std::monostate{};
				}

				_compMap.at(ent).clear();

				break;
			}
			++location;
		}
	}
}