#pragma once

#include "Service.hpp"
#include <any>
#include <unordered_map>
#include <string>


namespace pork::base
{

class Config : public Service<Config>
{
public:
    enum class AccessMode
    {
        READ_ONLY,
        READ_WRITE
    };

    using Location = std::pair<AccessMode, std::size_t>;
    using LocationByName = std::unordered_map<std::string, Location>;

public:
    Config() = default;
    ~Config() = default;


    inline bool hasValue(std::string const & name) const
    {
        return mLocationByName.contains(name);
    }


    template<typename ValueType>
    void setValue(std::string const & name,
                  ValueType const && value,
                  AccessMode accessMode = AccessMode::READ_WRITE)
    {
        auto const locationIt = mLocationByName.find(name);
        if (locationIt != mLocationByName.end()
            && locationIt->second.first == AccessMode::READ_ONLY)
        {
            // TODO: assert here
            return;
        }

        auto entity = std::make_any<ValueType>(value);

        if (locationIt == mLocationByName.end())
        {
            addEntity(name, std::move(entity), accessMode);
            return;
        }

        getEntity(locationIt->second)->swap(entity);
    }

    template<typename ValueType>
    ValueType * getValue(std::string const & name) const
    {
        auto * entity{getEntity(name)};
        if (!entity)
        {
            return nullptr;
        }

        return std::any_cast<ValueType>(entity);
    }


    template<typename ValueType>
    void getValue(std::string const & name, ValueType & value) const
    {
        auto * entity{getEntity(name)};
        if (!entity)
        {
            return;
        }

        value = *(std::any_cast<ValueType>(entity));
    }


private:
    void addEntity(std::string const & name,
                   std::any && entity,
                   AccessMode accessMode = AccessMode::READ_WRITE)
    {
        std::size_t idx{0u};
        if (accessMode == AccessMode::READ_ONLY)
        {
            idx = mROEntities.size();
            mROEntities.emplace_back(std::move(entity));
        }
        else
        {
            idx = mRWEntities.size();
            mRWEntities.emplace_back(std::move(entity));
        }

        Location loc{accessMode, idx};
        mLocationByName.emplace(name, loc);
    }

    std::any * getEntity(std::string const & name)
    {
        auto const locationIt = mLocationByName.find(name);
        if (locationIt == mLocationByName.end())
        {
            // TODO: assert here
            return nullptr;
        }


        return getEntity(locationIt->second);
    }

    std::any * getEntity(Location const & location)
    {
        std::any * entity{nullptr};
        if (location.first == AccessMode::READ_ONLY)
        {
            entity = &(mROEntities.at(location.second));
        }
        else
        {
            entity = &(mRWEntities.at(location.second));
        }

        return entity;
    }

private:
    std::vector<std::any> mROEntities;
    std::vector<std::any> mRWEntities;
    LocationByName mLocationByName;
};

} // namespace pork::base
