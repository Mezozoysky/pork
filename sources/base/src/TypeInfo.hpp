#pragma once

#include <string>
#include <typeindex>
#include <unordered_map>

namespace pork::base
{

class TypeInfo
{
public:
    TypeInfo() noexcept;
    explicit TypeInfo(std::type_index index, TypeInfo const * const baseTypeInfo, char const * const typeName) noexcept;
    virtual ~TypeInfo() noexcept = default;

    bool isValid() const noexcept
    {
        return (mIndex != typeid(std::nullptr_t));
    }

    inline operator bool() const noexcept
    {
        return isValid();
    }

    inline std::type_index typeIndex() const noexcept
    {
        return mIndex;
    }

    inline char const * const typeName() const noexcept
    {
        return mTypeName;
    }

    inline TypeInfo const * const baseTypeInfo() const noexcept
    {
        return mBaseInfo;
    }

private:
    std::type_index mIndex;
    char const * const mTypeName;
    TypeInfo const * const mBaseInfo;
};


template<typename DerivedT, typename BaseT, char const * const typeNameV>
class TypeInfoMixin
{
    static_assert(std::is_base_of<BaseT, DerivedT>::value, u8"TypeInfoMixin: invalid base type");
public:
    using Type = DerivedT;
    using BaseType = BaseT;//DerivedT::BaseType; /// "using BaseType = ..." is required within DerivedT

public:
    static TypeInfo const * const getTypeInfoStatic()
    {
        return &sTypeInfo;
    }

    inline static char const * const typeNameStatic() noexcept
    {
        return sTypeInfo.typeName();
    }

public:
    TypeInfoMixin() noexcept = default;
    virtual ~TypeInfoMixin() noexcept = default;

    inline TypeInfo const * const getTypeInfo() const noexcept
    {
        return &sTypeInfo;
    }

    inline char const * const typeName() const noexcept
    {
        return sTypeInfo.typeName();
    }

private:
    static TypeInfo sTypeInfo;
};


template<typename DerivedT, typename BaseT, char const * const typeNameV>
TypeInfo TypeInfoMixin<DerivedT, BaseT, typeNameV>::sTypeInfo{{typeid(DerivedT)}, BaseT::getTypeInfoStatic(), typeNameV};

} // namespace pork::base
