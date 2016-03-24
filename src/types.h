#pragma once

#include "common.h"

#include "ecs/core.h"

namespace engine {
namespace types {

	class IObject
	{
	public:
		static constexpr auto c_obj_interfaceName = "IObject";

		virtual ~IObject();

		virtual ecs::EntityId object_type() = 0;
		virtual std::string object_name() = 0;
	};

	class IObjectComposite : public virtual IObject
	{
	public:
		static constexpr auto c_obj_interfaceName = "IObjectComposite";

		struct Listing
		{
			std::string name;
		};

		virtual std::vector<IObject*> const& objectComposite_parts() const = 0;
		virtual Listing const& objectComposite_getListing(IObject const*) const = 0;

		/* T:
		Provides the objectComposite's signal for when the parts collection is updated. It has a
		couple of different call styles:

			* Add new object: when (objNew != nullptr, objOld == nullptr).
			* Rem old object: when (objNew == nullptr, objOld != nullptr).
			* Replace object: when (objNew == nullptr, objOld == nullptr). Replaces old with new.
		*/
		virtual Signal<void (IObject* objNew, IObject* objOld)>& objectComposite_onUpdate() = 0;
	};

	class IObjectManipulation : public virtual IObject
	{
	public:
		static constexpr auto c_obj_interfaceName = "IObjectManipulation";

		typedef void* listingId_t;
		struct Listing
		{
			listingId_t id;
			std::string name;
			ecs::EntityId ret;
			std::vector<ecs::EntityId> args;
		};

		virtual IObject* objectManipulation_call(listingId_t, std::vector<IObject*> const&) const = 0;
		virtual std::vector<Listing> const& objectManipulation_listings(IObject const*) const = 0;
	};

	template<class TType>
	struct CTypeInterfaceDescriptor
	{
		static constexpr auto c_ecs_componentName = TType::c_obj_interfaceName;

		ptrdiff_t offset;

		inline TType* cast(IObject* o) const
		{
			return reinterpret_cast<TType*>(reinterpret_cast<std::uintptr_t>(o) + offset);
		}
		inline TType const* cast(IObject const* o) const
		{
			return reinterpret_cast<TType const*>(reinterpret_cast<std::uintptr_t>(o) + offset);
		}
	};

	template<class TType>
	using
		InterfaceManager = ecs::FastComponentManager<CTypeInterfaceDescriptor<TType>>;

	class TypesEcs : public ecs::System<
		ecs::SystemConfig<>,
		InterfaceManager<IObjectComposite>,
		InterfaceManager<IObjectManipulation>
	>
	{

	};

}
}