#pragma once

#include "common.h"

/* T:
Describes our descoverable types.
*/

enum TypeTag
{
	TypeTag_Unknown = 0,

	// These are type flags
	/* T:
	Describes a complex composite object which contains other types.
	*/
	TypeTag_FlagComposite = 1,

	/* T:
	Describes a complex object exposing dynamic features.
	*/
	TypeTag_FlagFeatures = 2,

	/* T:
	Anything >= this value is not a flagged value.
	*/
	TypeTag_NOTFLAGGED = 16,

	/* T:
	Describes special value string:
	*/
	TypeTag_SpecialLiteralString = 16,

	// Actual type list, see the corresponding interfaces.

	TypeTag_InputMapping = 32, // IInputMapping
	TypeTag_Asset = 33, // IAsset

	TypeTag_USER = 1024, // First User Section
};

struct AbstractTypeTag
{
	TypeTag type;
	void* object;
};

class ICompositeType abstract : public virtual AbstractTypeTag
{
public:
	struct CompositeListing
	{
		AbstractTypeTag const* tag;
		std::string name;
	};

	virtual std::vector<CompositeListing> const& CompositeType_values() const = 0;
	void* CompositeType_nextObject;

	inline bool CompositeType_has(std::string const& name)
	{
		auto values = this->CompositeType_values();
		for (auto l = values.begin(); l != values.end(); l++)
		{
			if (name == l->name)
				return true;
		}
		return false;
	}

	inline AbstractTypeTag const* CompositeType_get(std::string const& name)
	{
		auto values = this->CompositeType_values();
		for (auto l = values.begin(); l != values.end(); l++)
		{
			if (name == l->name)
				return l->tag;
		}
		return nullptr;
	}
};

class IFeaturesType abstract : public virtual AbstractTypeTag
{
public:
	struct FeatureArgumentListing
	{
		std::string name;
		std::string description;
	};

	struct FeatureListing
	{
		std::string name;
		std::string description;

		FeatureArgumentListing ret;
		std::vector<FeatureArgumentListing> args;

		std::function<AbstractTypeTag* (std::vector<AbstractTypeTag*>)> call;
	};

	virtual std::vector<FeatureListing> const& FeaturesType_values() const = 0;
	void* FeaturesType_nextObject;

	inline bool FeaturesType_has(std::string const& name)
	{
		auto values = this->FeaturesType_values();
		for (auto l = values.begin(); l != values.end(); l++)
		{
			if (name == l->name)
				return true;
		}
		return false;
	}

	inline FeatureListing const* FeaturesType_get(std::string const& name)
	{
		auto values = this->FeaturesType_values();
		for (auto l = values.begin(); l != values.end(); l++)
		{
			if (name == l->name)
				return &(*l);
		}
		return nullptr;
	}
};

class GenericComplexType :
	public ICompositeType,
	public IFeaturesType,
	public virtual AbstractTypeTag
{
	std::vector < CompositeListing > _composite_listing;
	std::vector < FeatureListing > _features_listing;

public:
	GenericComplexType();

	virtual std::vector<CompositeListing> const& CompositeType_values() const;
	virtual std::vector<FeatureListing> const& FeaturesType_values() const;

	// True if success
	bool CompositeType_add(std::string const& name, AbstractTypeTag const& tag);
	bool CompositeType_remove(std::string const& name);
	bool FeaturesType_add(FeatureListing const& listing);
	bool FeaturesType_remove(std::string const& name);
};
