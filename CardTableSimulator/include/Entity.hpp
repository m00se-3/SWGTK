#ifndef CTS_ENTITY_HPP
#define CTS_ENTITY_HPP

#include <string>
#include <unordered_map>
#include <map>
#include <list>

#include "Component.hpp"

namespace cts
{
	using Entity = uint32_t;

	enum class EntityError : uint32_t
	{
		no_storage
	};

	class Registry
	{
	public:
		using CreateResult = std::variant<std::monostate, Entity, EntityError>;

		Registry() = default;

		[[nodiscard]] Registry::CreateResult Create();
		void SetComponentCapacity(const std::string& name, uint32_t size);
		void Destroy(Entity entity);

		template<typename CompType>
		void RegisterComponent(const std::string& name)
		{
			if (!_compContainer.contains(name))	// Assuming the component is not already registered.
			{
				_compContainer.insert_or_assign(name, std::vector<Component::Type>{});
				_freeLists.insert_or_assign(name, std::list<uint32_t>{});
			}
		}

		template<typename CompType, typename... Args>
		void AddComponent(Entity entity, const std::string& name, Args&&... args)
		{
			auto& ent = _compMap.at(entity);
			auto& free = _freeLists.at(name);
			
			if (_compContainer.contains(name) && !ent.contains(name))	// Assuming the component is already registered.
			{
				auto& component = _compContainer.at(name);

				/*
					If the free list is empty, we place the component at the back of the container and attach it to the entity
				*/
				if (free.empty())
				{
					auto place = component.size();

					component.emplace_back(CompType{ args... });
					ent.insert_or_assign(name, place);
				}
				else
				{
					// Otherwise, we use the first free component slot.
					
					auto& place = free.front();

					component[place] = CompType{ args... };
					ent[name] = place;

					free.pop_front();
				}
			}
		}

		template<typename CompType>
		void RemoveComponent(Entity entity, const std::string& name)
		{
			auto& ent = _compMap.at(entity);

			if (_compContainer.contains(name) && !ent.contains(name))	// Assuming the component is already registered.
			{
				_freeLists.at(name).emplace_back(ent.at(name));
				ent.erase(name);
			}
		}


	private:
		
		std::vector<Entity> _entities;
		std::list<Entity> _freeEntities;

		std::map<Entity, std::unordered_map<std::string, uint32_t>> _compMap;
		std::unordered_map<std::string, std::vector<Component::Type>> _compContainer;
		std::unordered_map<std::string, std::list<uint32_t>> _freeLists;

		static uint32_t _entityIDCounter;
	};

}

#endif // !CTS_ENTITY_HPP
