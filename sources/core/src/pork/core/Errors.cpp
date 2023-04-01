#include "Errors.hpp"


namespace pork::core
{

NotImplementedError::NotImplementedError(char const * message, int code)
: Exception(message, code)
{}

} // namespace pork::core
