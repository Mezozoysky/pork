#include "TypeInfo.hpp"

namespace pork::core
{

TypeInfo::TypeInfo() noexcept
: mIndex{typeid(std::nullptr_t)}
, mBaseInfo{nullptr}
, mTypeName{}
{}

TypeInfo::TypeInfo(std::type_index index, TypeInfo const * const baseInfo, char const * const typeName) noexcept
: mIndex{index}
, mBaseInfo{baseInfo}
, mTypeName{typeName}
{}


} // namespace pork::core
