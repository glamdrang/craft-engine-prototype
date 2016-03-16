#include "common.h"
#include "types.h"

GenericComplexType::GenericComplexType()
{
	type = (TypeTag)(TypeTag_FlagComposite | TypeTag_FlagFeatures);
	object = dynamic_cast<ICompositeType*>(this);
	CompositeType_nextObject = dynamic_cast<IFeaturesType*>(this);
	FeaturesType_nextObject = nullptr;
}

std::vector<ICompositeType::CompositeListing const> const& GenericComplexType::CompositeType_values() const
{
	return this->_composite_listing;
}

std::vector<IFeaturesType::FeatureListing const> const& GenericComplexType::FeaturesType_values() const
{
	return this->_features_listing;
}

bool GenericComplexType::CompositeType_add(std::string const& name, AbstractTypeTag const& tag)
{
	if (this->CompositeType_has(name))
		return false;

	ICompositeType::CompositeListing listing;

	listing.tag = &tag;
	listing.name = name;

	this->_composite_listing.push_back(listing);
	return true;
}

bool GenericComplexType::CompositeType_remove(std::string const& name)
{
	auto values = this->CompositeType_values();
	for (auto l = values.begin(); l != values.end(); l++)
	{
		if (name == l->name)
		{
			values.erase(l);
			return true;
		}
	}

	return false;
}

bool GenericComplexType::FeaturesType_add(FeatureListing const& listing)
{
	if (this->FeaturesType_has(listing.name))
		return false;

	this->_features_listing.push_back(listing);
	return true;
}

bool GenericComplexType::FeaturesType_remove(std::string const& name)
{
	auto values = this->FeaturesType_values();
	for (auto l = values.begin(); l != values.end(); l++)
	{
		if (name == l->name)
		{
			values.erase(l);
			return true;
		}
	}

	return false;
}