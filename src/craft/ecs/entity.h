#pragma once
#include <cstdint>

namespace ecs
{
	// This is our entity ID type.
	typedef unsigned long long EntityId;

	class SimpleEntityAllocator
	{
		EntityId _topEntity;
		std::set<EntityId> _reclaimedSet;
	public:
		inline SimpleEntityAllocator();

		inline EntityId entity_alloc();
		inline size_t entity_count();
		inline bool entity_exists(EntityId const& e);
		inline void entity_reclaim(EntityId e);
	};

	/* TODO: Add type detectors, see `util/templates.h`.
	template <typename T>
	using entity_alloc_t = decltype(std::declval<EntityId>() = std::declval<T>().entity_alloc());

	template<typename T, typename = void_t<>>
	struct is_entity_allocator : std::false_type { };

	template<typename T>
	struct is_entity_allocator<T,
		_t::detect<T, entity_alloc_t> >
	> {
	};
	*/
}

/******************************************************************************
** SimpleEntityAllocator
******************************************************************************/

ecs::SimpleEntityAllocator::SimpleEntityAllocator()
{
	this->_topEntity = 1;
}

ecs::EntityId ecs::SimpleEntityAllocator::entity_alloc()
{
	if (_reclaimedSet.size() != 0)
	{
		auto it = _reclaimedSet.begin();
		EntityId i = *it;
		_reclaimedSet.erase(it);
		return i;
	}

	return this->_topEntity++;
}

size_t ecs::SimpleEntityAllocator::entity_count()
{
	return this->_topEntity - (_reclaimedSet.size() + 1);
}

inline bool ecs::SimpleEntityAllocator::entity_exists(ecs::EntityId const& e)
{
	if (e == 0 || e >= this->_topEntity || _reclaimedSet.find(e) != _reclaimedSet.end())
	{
		return false;
	}
	return true;
}

void ecs::SimpleEntityAllocator::entity_reclaim(ecs::EntityId e)
{
	if (!this->entity_exists(e))
	{
		// TODO throw an exception
		return;
	}

	_reclaimedSet.insert(e);
}
