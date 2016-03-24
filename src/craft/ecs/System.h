#pragma once
#include "common.h"

#include "ecs/core.h"

namespace ecs
{
	template<
		typename TEntityAllocator = SimpleEntityAllocator
	>
	class SystemConfig
	{
		//static_assert(std::is_entity_allocator<TEntityAllocator>::value, "TEntityAllocator must implement entity allocation.");
	private: // Type holder only
		SystemConfig() = delete;
		SystemConfig(SystemConfig const&) = delete;
		SystemConfig(SystemConfig const&&) = delete;
	public:
		typedef typename TEntityAllocator T_entityAllocator;
	};

	template<
		class TSystemConfig,
		class... TManagers
	>
	class System;

	template<
		typename... TSystemConfigs
	>
	class System < SystemConfig<TSystemConfigs...> >
	{
	protected:
		typedef typename SystemConfig<TSystemConfigs...> T_systemConfig;
		typedef typename T_systemConfig::T_entityAllocator T_alloc;

		T_alloc alloc;
	public:
		const static size_t c_managerCount = 0;
	public:
		// Entities
		inline size_t entityCount()
		{
			return alloc.entity_count();
		}

		inline EntityId e()
		{
			return alloc.entity_alloc();
		}
		inline bool e_has(EntityId const& e)
		{
			return alloc.entity_exists(e);
		}
		inline void e_rem(EntityId& e)
		{
			// TODO rem components
			alloc.entity_reclaim(e);
			e = 0;
		}

		// Default (error) implementations
		template <typename TComponent>
		inline TComponent& c(EntityId const& e)
		{ static_assert(_T_assertBad<TComponent>::value, "Component Implementation Not Found, <see above>."); }

		template <typename TComponent>
		inline TComponent const& c_get(EntityId const& e)
		{ static_assert(_T_assertBad<TComponent>::value, "Component Implementation Not Found, <see above>."); }

		template <typename TComponent>
		inline void c_set(EntityId const& e, TComponent const&)
		{ static_assert(_T_assertBad<TComponent>::value, "Component Implementation Not Found, <see above>."); }

		template <typename TComponent>
		inline bool c_has(EntityId const& e)
		{ static_assert(_T_assertBad<TComponent>::value, "Component Implementation Not Found, <see above>."); }

		template <typename TComponent>
		inline void c_rem(EntityId const& e)
		{ static_assert(_T_assertBad<TComponent>::value, "Component Implementation Not Found, <see above>."); }
	};

	template<
		class TSystemConfig,
		class TManager,
		class... TRestManagers
	>
	class System<TSystemConfig, TManager, TRestManagers...>
		: public System <TSystemConfig, TRestManagers...>
	{
		typedef typename System < TSystemConfig, TRestManagers... > T_base;
		typedef typename TManager::T_component T_curComponent;

		TManager _curManager;
	public:
		const static size_t c_managerCount = T_base::c_managerCount + 1;
		
		inline static size_t managerCount()
		{
			return c_managerCount;
		}

		// Forwading implementations
		template <typename T>
		inline T& c(EntityId const& e) { return T_base::c<T>(e); }

		template <typename T>
		inline T const& c_get(EntityId const& e) { return T_base::c_get<T>(e); }

		template <typename T>
		inline void c_set(EntityId const& e, T const& c) { T_base::c_set<T>(e, c); }

		template <typename T>
		inline bool c_has(EntityId const& e) { return T_base::c_has<T>(e); }

		template <typename T>
		inline void c_rem(EntityId const& e) { T_base::c_rem<T>(e); }

		// Current manager implementations
		template <>
		inline T_curComponent& c<T_curComponent>(EntityId const& e)
		{ return _curManager.component_mut(e); }

		template <>
		inline T_curComponent const& c_get<T_curComponent>(EntityId const& e)
		{ return _curManager.component_get(e); }

		template <>
		inline void c_set<T_curComponent>(EntityId const& e, T_curComponent const& c)
		{ _curManager.component_set(e, c); }

		template <>
		inline bool c_has<T_curComponent>(EntityId const& e)
		{ return _curManager.component_has(e); }

		template <>
		inline void c_rem<T_curComponent>(EntityId const& e)
		{ _curManager.component_rem(e); }
	};
}
