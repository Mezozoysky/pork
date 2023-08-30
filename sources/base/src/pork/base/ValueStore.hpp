#pragma once

#include <any>
#include <unordered_map>
#include <string>


namespace pork::base
{

class ValueStore
{
public:
    enum class Access
    {
        READ_ONLY,
        READ_WRITE
    };

private:
    using Location = std::pair<Access, std::size_t>;
    using LocationByName = std::unordered_map<std::string, Location>;

public:
    ValueStore() = default;
    ~ValueStore() = default;


    inline bool has(std::string const & name) const;

    template<typename ValueType>
    void set(std::string const & name,
             ValueType const && value,
             Access access = Access::READ_WRITE);

    template<typename ValueType>
    ValueType const * get(std::string const & name) const;

    template<typename ValueType>
    void get(std::string const & name, ValueType & value) const;


private:
    void addEntity(std::string const & name,
                   std::any && entity,
                   Access access = Access::READ_WRITE);
    std::any const * const getEntity(std::string const & name) const;
    std::any * getEntity(Location const & location);
    std::any const * const getEntity(Location const & location) const;

private:
    std::vector<std::any> mROEntities;
    std::vector<std::any> mRWEntities;
    LocationByName mLocationByName;
};


inline bool ValueStore::has(std::string const & name) const
{
    return mLocationByName.contains(name);
}


template<typename ValueType>
void ValueStore::set(std::string const & name, ValueType const && value, Access access)
{
    auto const locationIt = mLocationByName.find(name);
    if (locationIt != mLocationByName.end() && locationIt->second.first == Access::READ_ONLY)
    {
        // TODO: assert here
        return;
    }

    auto entity = std::make_any<ValueType>(value);

    if (locationIt == mLocationByName.end())
    {
        addEntity(name, std::move(entity), access);
        return;
    }

    getEntity(locationIt->second)->swap(entity);
}


template<typename ValueType>
ValueType const * ValueStore::get(std::string const & name) const
{
    std::any const * entity{getEntity(name)};
    if (!entity)
    {
        return nullptr;
    }

    return std::any_cast<ValueType>(entity);
}


template<typename ValueType>
void ValueStore::get(std::string const & name, ValueType & value) const
{
    std::any const * entity{getEntity(name)};
    if (!entity)
    {
        return;
    }

    value = *(std::any_cast<ValueType>(entity));
}

} // namespace pork::base
