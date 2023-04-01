#include "Exception.hpp"


namespace pork::core
{

class NotImplementedError
: public Exception
{
public:
    NotImplementedError(char const * message, int code = -100);
};

} // namespace pork::core
