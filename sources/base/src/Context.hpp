#pragma once

#include "Service.hpp"
#include <unordered_map>
#include <typeindex>
#include <stdexcept>
#include <string>
#include <type_traits>


namespace pork::base
{

class Context
{
    using TypeMap = std::unordered_map<std::type_index, AbstractService::Ptr>;

public:
    Context() = default;

    template<typename ServiceT, typename... ArgsT>
    ServiceT * addService(ArgsT... args)
    {
        std::type_index index{typeid(ServiceT)};
        if (mServices.find(index) != mServices.end())
        {
            throw std::runtime_error{std::string{"Failed to add service: "} + index.name() + " already exists"};
        }
        return static_cast<ServiceT *>(addService(std::move(index), ServiceT::makePtr(std::forward<ArgsT>(args...)...)));
    }
    template<typename ServiceT>
    ServiceT * addService(typename ServiceT::Ptr && ptr)
    {
        std::type_index index{typeid(ServiceT)};
        if (mServices.find(index) != mServices.end())
        {
            throw std::runtime_error{std::string{"Failed to add service: "} + index.name() + " already exists"};
        }
        return static_cast<ServiceT *>(addService(std::move(index), std::move(ptr)));
    }

    template<typename ServiceT>
    void removeService()
    {
        removeService({typeid(ServiceT)});
    }

    template<typename ServiceT>
    inline ServiceT * service()
    {
        return static_cast<ServiceT *>(service({typeid(ServiceT)}));
    }

private:
    AbstractService * addService(std::type_index && index, AbstractService::Ptr && service);
    void removeService(std::type_index && index);
    AbstractService * service(std::type_index && index) const;

private:
    TypeMap mServices;
};


class Contextual
{
public:
    Contextual(Context & ctx);
    virtual ~Contextual() = default;

    Context & context() const;

private:
    Context & mCtx;
};


inline Contextual::Contextual(Context & ctx)
: mCtx{ctx}
{
}

inline Context & Contextual::context() const
{
    return mCtx;
}

} // namspace pork::base

