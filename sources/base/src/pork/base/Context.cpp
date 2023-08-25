#include "Context.hpp"
#include <string>
#include <stdexcept>


namespace pork::base
{

AbstractService * Context::addService(std::type_index && index, AbstractService::Ptr && service)
{
    auto p = mServices.emplace(std::move(index), std::move(service));
    if (!p.second)
    {
        return nullptr;
    }
    return p.first->second.get();
}

void Context::removeService(std::type_index && index)
{
    auto p = mServices.find(index);
    if (p != mServices.end())
    {
        mServices.erase(index);
    }
}

AbstractService * Context::service(std::type_index && index) const
{
    auto p = mServices.find(index);
    return p != mServices.end() ? p->second.get() : nullptr;
}

} // namspace pork::base

