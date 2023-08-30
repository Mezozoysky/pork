#include "ValueStore.hpp"


namespace pork::base
{

void ValueStore::addEntity(std::string const & name, std::any && entity, Access access)
{
    std::size_t idx{0u};
    if (access == Access::READ_ONLY)
    {
        idx = mROEntities.size();
        mROEntities.emplace_back(std::move(entity));
    }
    else
    {
        idx = mRWEntities.size();
        mRWEntities.emplace_back(std::move(entity));
    }

    Location loc{access, idx};
    mLocationByName.emplace(name, loc);
}


std::any const * const ValueStore::getEntity(std::string const & name) const
{
    auto const locationIt = mLocationByName.find(name);
    if (locationIt == mLocationByName.end())
    {
        return nullptr;
    }

    return getEntity(locationIt->second);
}


std::any * ValueStore::getEntity(Location const & location)
{
    if (location.first == Access::READ_ONLY)
    {
        return &(mROEntities.at(location.second));
    }
    else
    {
        return &(mRWEntities.at(location.second));
    }
}


std::any const * const ValueStore::getEntity(Location const & location) const
{
    if (location.first == Access::READ_ONLY)
    {
        return &(mROEntities.at(location.second));
    }
    else
    {
        return &(mRWEntities.at(location.second));
    }
}

} // namespace pork::base
