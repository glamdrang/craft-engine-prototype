#pragma once

namespace ecs
{
	template <typename T>
	class IComponentManager
	{
		inline T& component_mut(EntityId const& e) { static_assert(false, "Not Implemented"); return 0; }
		inline T const& component_get(EntityId const& e) { static_assert(false, "Not Implemented"); return 0; }
		inline void component_set(EntityId const& e, T const& data) { static_assert(false, "Not Implemented"); }
		inline bool component_has(EntityId const& e) { static_assert(false, "Not Implemented"); return 0; }
		inline void component_rem(EntityId const& e) { static_assert(false, "Not Implemented"); }
	public:
		typedef typename T T_component;

		const static bool c_ecs_isFast = false;
		constexpr static auto c_ecs_compName = T::c_ecs_componentName;
		const static size_t c_ecs_compSize = sizeof(T);
	};

	template <typename T>
	class MapComponentManager : public IComponentManager<T>
	{
		std::map < EntityId, T > _components;
	public:
		MapComponentManager()
		{

		}

		inline T& component_mut(EntityId const& e)
		{
			return _components[e];
		}
		inline T const& component_get(EntityId const& e) const
		{
			return _components.at(e);
		}
		inline void component_set(EntityId const& e, T const& data)
		{
			_components[e] = data;
		}
		inline bool component_has(EntityId const& e)
		{
			return _components.find(e) != _components.end();
		}
		inline void component_rem(EntityId const& e)
		{
			_components.erase(e);
		}
	};

	template <typename T>
	class FastComponentManager : public MapComponentManager<T>
	{
	public:
		const static bool c_ecs_isFast = true;
	};
}
