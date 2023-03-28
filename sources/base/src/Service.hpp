#pragma once

#include <memory>


namespace pork::base
{

class AbstractService
{
public:
    using Ptr = std::unique_ptr<AbstractService>;

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

    template<typename... ArgsT>
    static Ptr makePtr(ArgsT... args)
    {
        return std::make_unique<ServiceT>(std::forward<ArgsT>(args)...);
    }

    Service() noexcept = default;
    virtual ~Service() noexcept = default;
};


// template<typename ServiceT, typename... ArgsT>
// inline typename ServiceT::Ptr AbstractService::make(ArgsT... args)
// {
// //    static_assert(std::is_base_of<Service<ServiceT>, ServiceT>::value, "TheService must be derived from Service<TheService>");
//     static_assert(std::is_base_of<AbstractService, ServiceT>::value, "TheService must be derived from AbstractService");
//     return std::make_unique<ServiceT>(std::forward<ArgsT>(args)...);
// }

} // namespace pork::base
