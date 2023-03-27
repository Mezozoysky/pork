#pragma once

#include <memory>


namespace pork::base
{

class AbstractService
{
public:
    using Ptr = std::unique_ptr<AbstractService>;

    template<typename ServiceT, typename... ArgsT>
    static typename ServiceT::Ptr make(ArgsT... args);

    AbstractService() noexcept = default;
    AbstractService(AbstractService const & other) = delete;
    AbstractService(AbstractService && other) = default;
    virtual ~AbstractService() noexcept = default;
    AbstractService & operator=(AbstractService const & other) = delete;
    AbstractService & operator=(AbstractService && other) = default;
};

template<typename ServiceT>
class Service
: public AbstractService
{
public:
    using Ptr = std::unique_ptr<ServiceT>;

    Service() noexcept = default;
    virtual ~Service() noexcept = default;
};


template<typename ServiceT, typename... ArgsT>
inline typename ServiceT::Ptr AbstractService::make(ArgsT... args)
{
    static_assert(std::is_base_of<Service<ServiceT>, ServiceT>::value, "TheService must be derived from Service<TheService>");
    return std::make_unique<ServiceT>(std::forward<ArgsT>(args)...);
}

} // namespace pork::base
