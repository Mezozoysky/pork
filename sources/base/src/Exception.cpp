#include "Exception.hpp"


namespace pork::base
{

Exception::Exception(char const * message, int code)
: std::runtime_error{message}
, mCode{code}
{}

} // namespace pork::base
