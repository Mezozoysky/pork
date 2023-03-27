#pragma once

#include <map>
#include <string>
#include <any>
#include <algorithm>
#include <cassert>


namespace sfmk::base
{

class Configuration
{
public:
    Configuration() = default;
    virtual ~Configuration() noexcept = default;

    template<typename TypeT>
    void set(std::u8string_view key, TypeT && value)
    {
        assert(!key.empty() && "Key must not be empty");
        if (key.empty())
        {
            spdlog::error("Failed to set configuration value: empty key");
            return;
        }
        auto itLeaf = mLeafs.find(key);
        if (itLeaf == mLeafs.end()) // key exists and key is leaf
        {
            mLeafs.emplace({key}, {std::move(value)});
        }
        else
        {
            it->second.emplace<TypeT>(std::move(value));
        }

        /*
        auto separatorPos{key.find_first_of(u8'.')};
        if (separatorPos == std::u8string_view::npos)
        {
            auto upperIt = std::upper_bound(mKeys.begin(), mKeys.end(), key);
            if (upperIt == mKeys.end())
            {
                mKeys.emplace_back(key);
            }
            else
            {
                mKeys.emplace(upperIt, key);
            }
            mLeafs.emplace(key, std::move(value));
            return;
        }

        std::u8string_view head{key};
        head.remove_suffix(key.size() - lastSeparatorPos);
        std::u8string_view tail{key};
        tail.remove_prefix(lastSeparatorPos + 1);

        auto headIt = std::lower_bound(mKeys.begin(), mKeys.end(), head);
        if (headIt == mKeys.end())
        {
            mKeys.emplace_back(key);
        }
        else if (*headIt == key)
        {
            // skip
        }
        else // *headIt > head
        {
            mKeys.emplace(headIt, key);
        }
        */

        // std::size_t lastSeparatorPos{key.find_last_of(u8'.')};
        // if (lastSeparatorPos == std::u8string_view::npos)
        // {
        //     auto keyIt = std::lower_bound(mKeys.begin(), mKeys.end(), key);
        //     if (keyIt == mKeys.end())
        //     {
        //         mKeys.emplace_back(key);
        //     }
        //     // else if (*keyIt == key) // impossible when the key is not a leaf
        //     // {}
        //     else
        //     {
        //         mKeys.emplace(keyIt, key);
        //     }
        //     mLeafs.emplace(key, std::move(value));
        //     return;
        // }



        // auto subKeys = tokenizeKey(key);
        // {
        //     auto currKey = subKeys[0u];
        //     std::size_t subKeyIdx{0u};
        //     while (subKeyIdx < subKeys.size())
        //     {
        //         // do
        //         currKey += u8"." += subKeys[subKeyIdx];
        //         ++subKeyIdx;
        //     }
        // }
        // std::any wrapped{std::move(value)};
        // mLeafs.emplace(key, std::move(wrapped));
    }

    template<TypeT>
    TypeT get(std::u8string_view key, TypeT && defaultVal) const noexcept
    {
        auto const it = mLeafs.find(key);
        if (it != mLeafs.end())
        {
            std::any const & wrapped{it->second};
            if (wrapped.has_value())
            {
                if (wrapped.type() == typeid(TypeT))
                {
                    TypeT value{std::any_cast<TypeT>(wrapped)};
                    return std::move(value);
                }
                spdlog::error("Failed to get configuration value for '{}'"
                              + ": '{}' type requested but '{}' type contained",
                              std::to_string(key),
                              std::to_string(typeid(TypeT)),
                              std::to_string(wrapped.type());
            }
            // else
            // {
            //     // if hasn't value there is no error
            // }
        }
        return std::move(defaultVal);
    }

    template<TypeT>
    TypeT * get(std::u8string_view key) const noexcept
    {
        auto const it = mLeafs.find(key);
        if (it != mLeafs.end())
        {
            std::any const & wrapped{it->second};
            if (wrapped.has_value())
            {
                if (wrapped.type() == typeid(TypeT))
                {
                    TypeT * value{std::any_cast<TypeT>(&wrapped)};
                    return value;
                }
                spdlog::error("Failed to get* configuration value for '{}'"
                              + ": '{}' type requested but '{}' type contained",
                              std::to_string(key),
                              std::to_string(typeid(TypeT)),
                              std::to_string(wrapped.type()));
            }
            // else
            // {
            //     // if hasn't value there is no error
            // }
        }
        else
        {
            spdlog::error("Failed to get* configuration value for '{}': no leaf key found",
                          std::to_string(key));
        }
        return nullptr;
    }

    template<TypeT>
    TypeT const & operator[](std::u8string_view key) const
    {
        auto const it = mLeafs.find(key);
        if (it != mLeafs.end())
        {
            std::any const & wrapped{it->second};
            if (wrapped.has_value())
            {
                if (wrapped.type() == typeid(TypeT))
                {
                    auto const & value = *(std::any_cast<TypeT>(&wrapped));
                    return value;
                }
                spdlog::error("Failed to [] configuration value for '{}'"
                              + ": '{}' type requested but '{}' type contained",
                              std::to_string(key),
                              std::to_string(typeid(TypeT)),
                              std::to_string(wrapped.type()));
            }
            else
            {
                Exception("configuration value for '{}' is empty", std::to_string(key));
            }
        }
        else
        {
            spdlog::error("Failed to [] configuration value for '{}': no leaf key found",
                          std::to_string(key));
        }
        throw ConfigurationException("Failed to [] configuration value for '" + std::to_string(key) + "'");
    }

    // bool hasKey(std::u8string_view key) const noexcept
    // {
    //     auto it = std::lower_bound(mKeys.begin(), mKeys.end(), key);
    //     if (it != mKeys.end())
    //     {
    //         return (it->second == key);
    //     }
    //     return false;
    // }

    inline bool hasLeafKey(std::u8string_view key) const noexcept
    {
        return mLeafs.contains(key);
    }

private:
    std::vector<std::u8string> mKeys;
    std::map<std::u8string, std::any> mLeafs;
};


} // namespace sfmk::base
